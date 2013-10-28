<?php
// hook into qw_basics
add_filter('qw_basics', 'qw_basic_settings_row_style');
// add default row styles to the filter
add_filter('qw_row_styles', 'qw_row_styles_default', 0);
// add default row complete styles to the filter
add_filter('qw_row_complete_styles', 'qw_row_complete_styles_default');
// add default field styles to the filter
add_filter('qw_row_fields_styles', 'qw_row_fields_styles_default');

/*
 * Basic Settings
 */
function qw_basic_settings_row_style($basics){
  $basics['display_row_style'] = array(
    'title' => 'Row Style',
    'type' => 'display',
    'description' => 'How should each post in this query be presented?',
    'form_callback' => 'qw_basic_settings_row_style_form',
    'weight' => 0,
  );
  return $basics;
}

/*
 * Default Row Styles
 */
function qw_row_styles_default($row_styles){
  $row_styles['posts'] = array(
    'title' => 'Posts',
    'settings_callback' => 'qw_basic_settings_row_styles_posts_settings',
  );
  $row_styles['fields'] = array(
    'title' => 'Fields',
    'settings_callback' => 'qw_basic_settings_row_styles_fields_settings',
  );
  return $row_styles;
}

/*
 * Default Row Style 'Posts' options
 */
function qw_row_complete_styles_default($row_complete_styles){
  $row_complete_styles['complete'] = array(
    'title' => 'Complete Post',
  );
  $row_complete_styles['excerpt'] = array(
    'title' => 'Excerpt',
  );
  return $row_complete_styles;
}

/*
 * Default Row Style 'Fields' options
 */
function qw_row_fields_styles_default($row_fields_styles){
  $row_fields_styles['posts'] = array(
    'title' => 'Posts',
  );
  $row_fields_styles['fields'] = array(
    'title' => 'Fields',
  );
  return $row_styles;
}

/*
 * Form for selecting row style and settings
 */
function qw_basic_settings_row_style_form($basic, $options)
{
  $row_styles = qw_all_row_styles();
  ?>
    <select id="qw-display-type"
            name="<?php print $basic['form_prefix']; ?>[row_style]">
      <?php
        foreach($row_styles as $type => $row_style)
        { ?>
            <option value="<?php print $type; ?>"
                    <?php if($options['display']['row_style'] == $type) { print 'selected="selected"';} ?>>
              <?php print $row_style['title']; ?>
            </option>
          <?php
        }
      ?>
    </select>
    <p class="description"><?php print $basic['description']; ?></p>

    <!-- style settings -->
    <div id="row-style-settings">
      <?php
        foreach($row_styles as $type => $row_style){
          if (function_exists($row_style['settings_callback']))
          {
            $row_style['values'] = (isset($options['display']['row_style_settings'][$type])) ? $options['display']['row_style_settings'][$type] : array();
            ?>
            <div id="tab-row-style-settings-<?php print $row_style['hook_key']; ?>" class="qw-query-content">
              <span class="qw-setting-header"><?php print $row_style['title']; ?> Settings</span>
              <div class="qw-setting-group">
                <?php print $row_style['settings_callback']($row_style); ?>
              </div>
            </div>
            <?php
          }
        }
      ?>
    </div>
  <?php
}
/*
 * Post row style settings form
 */
function qw_basic_settings_row_styles_posts_settings($row_style)
{ ?>
    <p class="description">Select the amount of the post to be shown.</p>
    <select class="qw-title-ignore"
            name="<?php print $row_style['form_prefix']; ?>[size]">
      <option value="complete"
              <?php if($row_style['values']['size'] == "complete") { print 'selected="selected"';} ?>>
        Complete Post
      </option>
      <option value="excerpt"
              <?php if($row_style['values']['size'] == "excerpt") { print 'selected="selected"';} ?>>
        Excerpt
      </option>
    </select>
  <?php
}
/*
 * Field row style settings form
 */
function qw_basic_settings_row_styles_fields_settings($field_style)
{ ?>
  <p>No settings for fields.</p>
  <input type="hidden" name="<?php print $field_style['form_prefix']; ?>[placeholder]" value="none" />
  <?php
}
