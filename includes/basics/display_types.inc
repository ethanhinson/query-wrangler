<?php
// hook into qw_basics
add_filter('qw_basics', 'qw_basic_settings_display_types',0);
add_filter('qw_display_types', 'qw_basic_settings_display_types_default');

/*
 * Basic Settings
 */
function qw_basic_settings_display_types($basics){
  $basics['display_types'] = array(
    'title' => 'Display Types',
    'type' => 'display',
    'description' => 'Enabling query display types makes the query output available to users.',
    'form_callback' => 'qw_basic_settings_display_types_form',
    'weight' => 0,
  );
  return $basics;
}

/*
 * Some default display types
 */
function qw_basic_settings_display_types_default($display_types)
{
  /*$display_types['page'] = array(
    'title' => 'Page',
    'description' => 'Make the Page Path available for visitors of your website.',
    'help' => 'When you create a Page Query, you give that query a path (URI) to display on.
      After creating the query, you can visit that URI on your website to view the results.
      This is a great way to create new, complex pages on your Wordpress site.
      <br /><br />
      <strong><em>Pages do not work with the Default permalink structure found <a href="<?php print get_bloginfo('wpurl'); ?>/wp-admin/options-permalink.php">here</a>.</em></strong>',
  );*/
  $display_types['widget'] = array(
    'title' => 'Widget',
    'description' => 'Allow this query to be selected on the QW Widget form.',
    'help' => 'The Query Wrangler comes with a reusable Wordpress Widget that an be places in sidebars.
      When you create a query of the this type, that query becomes selectable in the Widget settings.',
  );
  return $display_types;
}
/*
 * Display types form
 */
function qw_basic_settings_display_types_form($basic, $options)
{ ?>
  <p class="description clear-left"><?php print $basic['description']; ?></p>
  <?php
    $display_types = qw_all_display_types();
    foreach ($display_types as $key => $display_type)
    {
      $checked = (isset($options['display']['types'][$key])) ? 'checked="checked"' : '';
      ?>
      <div class="clear-left">
        <label class="qw-label"><?php print $display_type['title']; ?>:</label>
        <input type="checkbox"
               name="<?php print $basic['form_prefix']; ?>[types][<?php print $key; ?>]"
               value="<?php print $key; ?>"
               <?php print $checked; ?> />
        <p class="description clear-left"><?php print $display_type['description']; ?></p>
      </div>
      <?php
    }
}