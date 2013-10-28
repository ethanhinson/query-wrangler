/*
 * Globals
 */
QueryWrangler.current_form_id = '';
QueryWrangler.new_form_id = '';
QueryWrangler.form_backup = '';
// changes have been made
QueryWrangler.changes = false;

(function ($) {
  
/*
 * Ajax preview
 */
QueryWrangler.get_preview = function() {
  // show throbber
  $('#query-preview-controls').removeClass('query-preview-inactive').addClass('query-preview-active');
  // serialize form data
  QueryWrangler.form_backup = $('form#qw-edit-query-form').serialize();
  // prepare post data
  var post_data_form = {
    'action': 'qw_form_ajax',
    'form': 'preview',
    'type': $('#preview-display-type').val(),
    'options': QueryWrangler.form_backup,
    'query_id': QueryWrangler.query.id
  };

  // make ajax call
  $.ajax({
    url: QueryWrangler.ajaxForm,
    type: 'POST',
    async: false,
    data: post_data_form,
    dataType: 'json',
    success: function(results){
      $('#query-preview-target').html(results.preview);
      $('#qw-show-arguments-target').html(results.args);
      $('#qw-show-display-target').html(results.display);
      $('#qw-show-wpquery-target').html(results.wpquery);
      $('#qw-show-query-time').html(results.time);
      $('#qw-show-template-files').html(results.templates);
    }
  });
  // hide throbber
  $('#query-preview-controls').removeClass('query-preview-active').addClass('query-preview-inactive');
}

/*
 * Make tokens for fields
 */
QueryWrangler.generate_field_tokens = function() {
  var tokens = [];
  $('#qw-query-field-list div.qw-query-title').each(function(i, item){
    // field name
    var field_name = $(item).attr('title').replace('qw-field-', '');
    // add tokens
    tokens.push('<li>{{'+field_name+'}}</li>');
    
    // target the field and insert tokens
    $('#qw-field-'+field_name+' ul.qw-field-tokens-list').html(tokens.join(""));
  }); 
}

/*
 * Init()
 */
$(document).ready(function(){
  // preview
  $('#get-preview').click(QueryWrangler.get_preview);
  QueryWrangler.get_preview();

  // accordions
  $('#query-details').accordion({
      header: '> div > .qw-setting-header',
			heightStyle: "content",
      collapsible: true,
      active: false,
      autoHeight: false
  });
});

})(jQuery);