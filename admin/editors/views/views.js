(function ($) {

QueryWrangler.set_modified = true;
QueryWrangler.dialogWidth = ($(window).width() * 0.7);
QueryWrangler.dialogHeight = ($(window).height() * 0.8);


/*
 * Move dialog boxes to the form
 */
QueryWrangler.restore_form = function(dialog){
  $(dialog).dialog('destroy');
  $(dialog).appendTo('#qw-options-forms').hide();
}

/*
 * Get templates
 * @param {String} handler - field or filter
 * @param {String} item_type the field or filter type
 */
QueryWrangler.get_handler_templates = function(handler, handler_hook, item_type){
  var item_count = $('#qw-options-forms input.qw-'+handler+'-type[value='+item_type+']').length;
  var next_name = (item_count > 0) ? item_type + "_" + item_count: item_type;
  var next_weight = $('ul#qw-'+handler+'-sortable li').length;

  // prepare post data for form and sortable form
  var post_data_form = {
    'action': 'qw_form_ajax',
    'form': handler+'_form',
    'name': next_name,
    'handler': handler,
    'hook_key': handler_hook,
    'type': item_type,
    'query_type': QueryWrangler.query.type,
    'next_weight': next_weight
  };

  // ajax call to get form
  $.ajax({
    url: QueryWrangler.ajaxForm,
    type: 'POST',
    async: false,
    data: post_data_form,
    success: function(results){
      // append the results
      $('#existing-'+handler+'s').append(results);
      QueryWrangler.add_list_item(post_data_form);
    }
  });

  QueryWrangler.toggle_empty_lists();

  return next_name;
}
QueryWrangler.add_list_item = function(post_data_form){

  var title = QueryWrangler.allHandlers[post_data_form.handler].all_items[post_data_form.hook_key].title;

  var output = "<div class='qw-query-title' title='qw-"+post_data_form.handler+"-"+post_data_form.name+"'>";
  output    +=   "<span class='qw-setting-title'>"+title+"</span> : ";
  output    +=   "<span class='qw-setting-value'>"+post_data_form.name+"</span>";
  output    += "</div>";

  $('#qw-query-'+post_data_form.handler+'-list').append(output);
}
/*
 * Dynamically set the setting title for updated fields
 */
QueryWrangler.set_setting_title = function(){
  //gather all info
  var form = $('#'+QueryWrangler.current_form_id);
  var settings = $('div[title='+QueryWrangler.current_form_id+']');
  var fields = form.find('input[type=text],input[type=checkbox],select,textarea,input[type=hidden]').not('.qw-weight').not('.qw-title-ignore');
  var new_title = [];
  var title_target = settings.children('.qw-setting-value');

  // fields
  if(settings.parent().attr('id') == 'qw-query-field-list'){
    new_title.push(form.find('input.qw-field-name').val());
  }

  // loop through the fields
  $.each(fields, function(i, field){
    // select
    if ($(field).is('select')){
      new_title.push( $(field).children('option[selected=selected]').text() );
    }
    // text field
    if ($(field).is('input[type=text]') &&
        $(field).val() != '')
    {
      new_title.push($(field).val());
    }
    // checkbox with value set
    if ($(field).is('input[type=checkbox]') &&
        $(field).is(':checked') &&
        $(field).val() != 'on')
    {
      new_title.push($(field).val());
    }
  });

  // single textarea fields, like header, footer, empty
  if(fields.length == 1 &&
     $(fields[0]).is('textarea'))
  {
    // in use
    if ($(fields[0]).val().trim() != ''){
      new_title.push('In Use');
    }
  }

  // no items found
  if (new_title.length == 0){
    new_title.push('None');
  }
  // title array to string
  new_title = new_title.join(', ');
  // set new title
  title_target.text(new_title);

  if (QueryWrangler.set_modified){
    title_target.parent().addClass('qw-modified');
  }
}

/*
 * Add new handler
 */
QueryWrangler.add_item = function(dialog){
  var handler = $(dialog).children('input.add-handler-type').val();
  $('#qw-display-add-'+handler+' input[type=checkbox]')
    .each(function(index,element){
      if($(element).is(':checked')){
        // item type
        var item_type = $(element).val();
        var handler_hook = $(element).siblings('input.qw-hander-hook_key').val();
        // add a new field
        var next_name = QueryWrangler.get_handler_templates(handler, handler_hook, item_type);
        // remove check
        $(element).removeAttr('checked');
      }
  });
  QueryWrangler.theme_accordions();
  //$(dialog).dialog('close');
}

QueryWrangler.theme_accordions = function(){
  if ($('#display-style-settings, #row-style-settings, .qw-sortable-list').hasClass('is-accordion')){
    $('#display-style-settings, #row-style-settings, .qw-sortable-list')
      .accordion('destroy');
  }
  $('#display-style-settings, #row-style-settings, .qw-sortable-list')
    .accordion({
      header: '> div > .qw-setting-header',
			heightStyle: "content",
      collapsible: true,
      active: false,
      autoHeight: false
  }).addClass('is-accordion');
}

QueryWrangler.toggle_empty_lists = function(){
  var lists = $(".qw-sortable-list");
  $.each(lists, function(){
    var num_items = $(this).children('.qw-sortable-item');
    if(num_items.length > 0)
    {
      $(this).children('.qw-empty-list').hide();
    }
    else
    {
      $(this).children('.qw-empty-list').show();
    }
  });
}
 /*
  * Update button
  */
QueryWrangler.button_update = function(dialog){
  var is_handler = false;
  var form = $('#'+QueryWrangler.current_form_id);

  // handlers have special needs
  $.each(QueryWrangler.handlers, function(i,handler){
    if (form.attr('id') == 'qw-display-add-'+handler+''){
      is_handler = true;
      return;
    }
  });

  // sortable handlers
  if (is_handler){
    QueryWrangler.add_item(dialog);
  }
  // normal titles
  else {
    // set the title for the updated field
    QueryWrangler.set_setting_title();
  }
  // clear the current form id
  QueryWrangler.current_form_id = '';

  // preview
  if($('#live-preview').is(':checked')){
    QueryWrangler.get_preview();
  }
  // changes
  if (QueryWrangler.changes == false){
    QueryWrangler.changes = true;
    $('.qw-changes').show();
  }
  //$(this).dialog('close');
}
/*
 * Cancel button
 */
QueryWrangler.button_cancel = function(){
  if(QueryWrangler.current_form_id != ''){
    // set backup_form
    $('form#qw-edit-query-form').unserializeForm(QueryWrangler.form_backup);
  }

  //$(this).dialog('close');
}
QueryWrangler.sortable_list_build = function(element){
  QueryWrangler.current_form_id = $(element).closest('.qw-query-admin-options').attr('id');

  var output = '<ul id="'+QueryWrangler.current_form_id+'-sortable" class="qw-hidden">';
  $('#'+QueryWrangler.current_form_id+'-list div').each(function(i, element){
    html = $(element).wrap('<div>').parent().html();
    output+= '<li class="qw-sortable ui-helper-reset ui-state-default ui-corner-all">'+html+'</li>';
  });
  output+= '</ul>';

  $(output).appendTo('#qw-options-forms');

  $('#'+QueryWrangler.current_form_id+'-sortable')
    .sortable()
    .dialog({
      modal: true,
      width: QueryWrangler.dialogWidth,
      height: QueryWrangler.dialogHeight,
      resizable: false,
      title: $(element).text(),
      close: function() {
        QueryWrangler.sortable_list_destroy(this);
      },
      buttons: [{
        text: 'Update',
        click: function() {
          QueryWrangler.sortable_list_update(this);
          QueryWrangler.sortable_list_destroy(this);
        }
      },{
        text: 'Cancel',
        click: function() {
          QueryWrangler.sortable_list_destroy(this);
        }
      }]
    });
}
QueryWrangler.sortable_list_destroy = function(dialog){
  //$(dialog).html('');
  $(dialog).dialog('destroy');
}

QueryWrangler.sortable_list_update = function(dialog){
  list_id = QueryWrangler.current_form_id+"-list";
  // empty list
  $('#'+list_id).html('');
  // loop through to repopulate list and update weights
  var items = $(dialog).children('.qw-sortable');
  $(items).each(function(i, item){
    // repopulate list
    $('#'+list_id).append($(item).html());
    // update weight
    var form_id = $(item).children('.qw-query-title').attr('title');
    // kitchen sink
    $('#'+form_id).find('.qw-weight').val(i).attr('value', i);
  });
  
  // Update Field tokens incase they were sorted
  QueryWrangler.generate_field_tokens();
  
  //QueryWrangler.sortable_list_update_weights(list_id);
}
QueryWrangler.sortable_list_update_weights = function(list_id){
  $(QueryWrangler.handlers).each(function(i, handler){
    $('#existing-'+handler+'s .qw-'+handler)
      .each(function(i, item){
        $(item).find(".qw-weight").attr('value', i);
        $(item).find(".qw-weight").val(i);
    });

    if (handler == 'field') {
      // Update Field tokens
      QueryWrangler.generate_field_tokens();
    }
  });
}

/*
 * Init()
 */
$(document).ready(function(){

  QueryWrangler.theme_accordions();
  QueryWrangler.toggle_empty_lists();
  QueryWrangler.sortable_list_update_weights();

  $('.qw-rearrange-title').click(function(){
    QueryWrangler.sortable_list_build(this);
  });

  $('#qw-query-admin-options-wrap').delegate('.qw-query-title', 'click', function(){
    // backup the form
    QueryWrangler.form_backup = $('form#qw-edit-query-form').serialize();

    QueryWrangler.current_form_id = $(this).attr('title');
    var dialog_title = $(this).text().split(':');

    $('#'+QueryWrangler.current_form_id).dialog({
      modal: true,
      width: QueryWrangler.dialogWidth,
      height: QueryWrangler.dialogHeight,
      title: dialog_title[0],
      resizable: false,
      close: function() {
        QueryWrangler.restore_form(this);
        QueryWrangler.button_cancel();
      },
      buttons: [{
        text: 'Update',
        click: function() {
          QueryWrangler.restore_form(this);
          QueryWrangler.button_update(this);
        }
      },{
        text: 'Cancel',
        click: function() {
          QueryWrangler.restore_form(this);
          QueryWrangler.button_cancel();
        }
      }]
    });
  });

  $('body.wp-admin').delegate('.qw-remove', 'click', function(){
    var title = $(this).closest('.ui-dialog-content').attr('id');
    // remove dialog
    $(this).closest('.ui-dialog').remove();
    // remove form item
    $('#'+title).remove();
    // remove list item
    $('.qw-query-title[title='+title+']').remove();
    QueryWrangler.toggle_empty_lists();
  });

  // fix settings titles
  QueryWrangler.set_modified = false;
  $('.qw-query-title').each(function(i,element){
    QueryWrangler.current_form_id = $(this).attr('title');
    QueryWrangler.set_setting_title();
  });
  QueryWrangler.set_modified = true;

});

})(jQuery);