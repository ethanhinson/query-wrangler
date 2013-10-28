<?php
/*
 * Ajax form templates
 */
function qw_form_ajax()
{
  /*
   * Preview, special case
   */
  if ($_POST['form'] == 'preview'){
    $decode = urldecode($_POST['options']);
    $options = array();
    parse_str($decode, $options);
    $options['qw-query-options']['args']['paged'] = 1;
    $options['qw-query-options']['display']['types']['this_instance'] = $_POST['type'];
    $args = array(
      'options' => $options['qw-query-options'],
      'query_id' => $_POST['query_id'],
      'templates' => qw_template_scan($options),
    );
    print theme('preview_ajax', $args);
    exit;
  }

  /*
   * Generate handler item forms and data
   */
  $handlers = qw_all_handlers();
  $handler = $handlers[$_POST['handler']];

  $item = array();

  $hook_key = qw_get_hook_key($handler['all_items'], $_POST); // kind of a trick
  $item = $handler['all_items'][$hook_key];

  $item['name'] = $_POST['name'];
  $item['form_prefix'] = $handler['form_prefix'].'['.$item['name'].']';

  if (isset($_POST['next_weight'])){
    $item['weight'] = $_POST['next_weight'];
  }

  if (!isset($item['form']) && function_exists($item['form_callback'])){
    ob_start();
      $item['form_callback']($item);
    $item['form'] = ob_get_clean();
  }

  $args = array(
    $_POST['handler'] => $item,
  );
  // weight for sortable handler items

  print theme($handler['wrapper_template'], $args);
  exit;
}


function qw_template_scan($options)
{
  global $wpdb;
  $query_id = $options['query-id'];
  $query = $wpdb->get_row('SELECT slug FROM '.$wpdb->prefix.'query_wrangler WHERE id ='.esc_sql($query_id));
  $slug = $query->slug;
  $options = $options['qw-query-options'];
  $all_styles = qw_all_styles();
  $all_row_styles = qw_all_row_styles();
  $style = $all_styles[$options['display']['style']];
  $row_style = $all_row_styles[$options['display']['row_style']];
  $output = array();
  $templates = array();

  // start building theme arguments
  $wrapper_args = array(
    'slug' => $slug,
    'type' => $options['display']['types']['this_instance'],
    'tw_action' => 'find_only',
  );
  // template with wrapper
  $templates['wrapper'] = theme('query_display_wrapper', $wrapper_args, true);

  $style_settings = array();
  if (isset($options['display']['style_settings'][$style['hook_key']])){
    $style_settings = $options['display']['style_settings'][$style['hook_key']];
  }
  // setup row template arguments
  $template_args = array(
    'template' => $style['template'],
    'slug' => $slug,
    'type' => $options['display']['types']['this_instance'],
    'style' => $style['hook_key'],
    'style_settings' => $style_settings,
    'tw_action' => 'find_only',
  );
  // template the query rows
  $templates['style'] = theme('query_display_rows', $template_args);

  if ($row_style['hook_key'] == "posts"){
    $row_style_settings = $options['display']['row_style_settings'][$row_style['hook_key']];

    $template_args = array(
      'template' => 'query-'.$row_style_settings['size'],
      'slug' => $slug,
      'type' => $options['display']['types']['this_instance'],
      'style' => $row_style_settings['size'],
      'tw_action' => 'find_only',
    );
    $templates['row_style'] = theme('query_display_rows', $template_args);
  }

  if ($row_style['hook_key'] == "fields"){
    $row_style_settings = $options['display']['row_style_settings'][$row_style['hook_key']];

    $template_args = array(
      'template' => 'query-field',
      'slug' => $slug,
      'type' => $options['display']['types']['this_instance'],
      'style' => $options['display']['row_style'],
      'tw_action' => 'find_only',
    );
    $templates['row_style'] = theme('query_display_rows', $template_args);
  }

  foreach($templates as $k => $template)
  {
    foreach($template['suggestions'] as $suggestion){
      if($suggestion == $template['found_suggestion']){
        $output[$k][] = '<strong>'.$suggestion.'</strong>';
      }
      else{
        $output[$k][] = $suggestion;
      }
    }

    // see if this is the default template
    if (stripos($template['found_path'], QW_PLUGIN_DIR) !== false){
      $output[$k]['found'] = '<em>(default) '.$template['found_path'].'</em>';
    } else {
      $output[$k]['found'] = '<strong>'.$template['found_path'].'</strong>';
    }
    //$output[$k]['template'] = $template;
  }

  return $output;
}