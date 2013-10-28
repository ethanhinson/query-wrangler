<?php

// CSS
function qw_admin_css(){
  print '<link rel="stylesheet" type="text/css" href="'.QW_PLUGIN_URL.'/admin/css/jquery-ui.css" />';
  print '<link rel="stylesheet" type="text/css" href="'.QW_PLUGIN_URL.'/admin/css/query-wrangler.css" />';
}
// admin list page
function qw_admin_list_js(){
  wp_enqueue_script('qw-admin-list-js',
                  plugins_url('/admin/js/query-wrangler-list.js', dirname(__FILE__)),
                  array(),
                  QW_VERSION,true);
}
// admin create page
function qw_admin_create_js(){
  wp_enqueue_script('qw-admin-create-js',
                  plugins_url('/admin/js/query-wrangler-create.js', dirname(__FILE__)),
                  array(),
                  QW_VERSION, true);
}
// Base JS
function qw_admin_js(){
  wp_enqueue_script('qw-admin-js',
                  plugins_url('/admin/js/query-wrangler.js', dirname(__FILE__)),
                  array('jquery-ui-core', 'jquery-ui-sortable', 'jquery-ui-dialog', 'jquery-ui-accordion'),
                  QW_VERSION, true);
  // jquery unserialize form
  wp_enqueue_script('qw-unserialize-form',
                  plugins_url('/admin/js/jquery.unserialize-form.js', dirname(__FILE__)),
                  array(),
                  QW_VERSION, true);
  $data = qw_edit_json();
  wp_localize_script( 'qw-admin-js',
                      'QueryWrangler',
                      array(
                        'l10n_print_after' => 'QueryWrangler = ' . $data. ';'
                      )
                    );
}
// Base Json data for query edit page
function qw_edit_json(){
  $handlers = qw_all_handlers();
  $handler_types = array();
  foreach($handlers as $type => $handler){
    $handler_types[] = $type;
  }
  $data = array(
    'handlers' => $handler_types,
    'ajaxForm' => admin_url( 'admin-ajax.php' ),
    'allHandlers' => $handlers,
    'allFields' => $handlers['field']['all_items'],
    'allFilters'  => $handlers['filter']['all_items'],
    'allSortOptions' => $handlers['sort']['all_items'],
    'allContextualFilters' => isset($handlers['contextual_filter']['all_items']) ? $handlers['contextual_filter']['all_items'] : array(),
    'allStyles' => qw_all_styles(),
    'allRowStyles' => qw_all_row_styles(),
    'allPostTypes' => qw_all_post_types(),
    'allPagerTypes' => qw_all_pager_types(),
    'allImageSizes' => get_intermediate_image_sizes(),
    'allFileStyles' => qw_all_file_styles(),
  );

  // editing a query
  if($query_id = $_GET['edit'])
  {
    // get the query
    global $wpdb;
    $table_name = $wpdb->prefix."query_wrangler";
    $sql = "SELECT name,type,data,path FROM ".$table_name." WHERE id = ".$query_id." LIMIT 1";
    $row = $wpdb->get_row($sql);

    $additional_data['query'] = array(
      'id' => $query_id,
      'options' => qw_unserialize($row->data),
      'name' => $row->name,
      'type' => $row->type,
    );

    $data = array_merge($data, $additional_data);
  }
  return json_encode( $data ) ;
}

/*
 * Handle the display of pages and actions
 */
function qw_page_handler(){
  // handle actions
  if(isset($_GET['action']))
  {
    $redirect = true;
    switch($_GET['action'])
    {
      case 'update':
        qw_update_query($_POST);
        // redirect to the edit page
        wp_redirect(get_bloginfo('wpurl').'/wp-admin/admin.php?page=query-wrangler&edit='.$_GET['edit']);
        break;

      case 'delete':
        qw_delete_query($_GET['edit']);
        // redirect to the list page
        wp_redirect(get_bloginfo('wpurl').'/wp-admin/admin.php?page=query-wrangler');
        break;

      case 'create':
        $new_query_id = qw_insert_new_query($_POST);
        // forward to the edit page
        wp_redirect(get_bloginfo('wpurl').'/wp-admin/admin.php?page=query-wrangler&edit='.$new_query_id);
        break;

      case 'import':
        $new_query_id = qw_query_import($_POST);
        // forward to edit page
        wp_redirect(get_bloginfo('wpurl').'/wp-admin/admin.php?page=query-wrangler&edit='.$new_query_id);
        break;

      case 'save_settings':
        qw_save_settings($_POST);
        // forward to edit page
        wp_redirect(get_bloginfo('wpurl').'/wp-admin/admin.php?page=qw-settings');
        break;
    }
  }

  // see if we're editng a page
  if(isset($_GET['edit']) &&
     is_numeric($_GET['edit']) &&
     !isset($redirect))
  {
    // show edit form
    qw_edit_query_form();
  }
  // export a query
  else if (isset($_GET['export']) && is_numeric($_GET['export'])){
    qw_export_page();
  }
  // else we need a list of queries
  else {
    include QW_PLUGIN_DIR.'/admin/page-list.php';
    qw_list_queries_form();
  }
}

/*
 * Query Edit Page
 */
function qw_edit_query_form(){
  global $wpdb;
  $table_name = $wpdb->prefix."query_wrangler";
  $query_id = $_GET['edit'];
  $sql = "SELECT name,type,data,path FROM ".$table_name." WHERE id = ".$query_id." LIMIT 1";
  $row = $wpdb->get_row($sql);

  $options = qw_unserialize($row->data);

  // handlers
  $handlers = qw_preprocess_handlers($options);

  // basic settings
  $basics = qw_all_basic_settings();

  foreach ($basics as $hook_key => $basic){
    // make the basic forms
    if (isset($basic['form_callback']) && function_exists($basic['form_callback'])){
      ob_start();
        $basic['form_callback']($basic, $options);
      $basics[$hook_key]['form'] = ob_get_clean();
    }
  }

  // editor theme
  $theme = get_option('qw_edit_theme');
  if($theme == ""){
    $theme = QW_DEFAULT_THEME;
  }

  // get rid of pesky slashes
  $options['display'] = array_map( 'stripslashes_deep', $options['display'] );

  // start building edit page data
  $edit_args = array(
    // editor theme
    'theme' => $theme,
    // query data
    'query_id' => $query_id,
    'options' => $options,
    'args'    => $options['args'],
    'display' => $options['display'],
    'query_name' => $row->name,
    'query_type' => $row->type,
    'query_page_title' => $options['display']['title'],
    'filters' => $handlers['filter']['items'],
    'fields' => $handlers['field']['items'],
    'sorts' => $handlers['sort']['items'],
    'basics' => $basics,
    'handlers' => $handlers,

    // all datas
    'all_fields' => $handlers['field']['all_items'],
    'all_filters'  => $handlers['filter']['all_items'],
    'all_sorts' => $handlers['sort']['all_items'],
  );

  // Page Queries
  if($row->type == 'page'){
    $edit_args['query_page_path'] = $row->path;
  }

  // overrides
  if($row->type == 'override'){
    $edit_args['query_override_type'] = $row->override_type;
  }

  // admin wrapper arguments
  $admin_args = array(
    'title' => 'Edit query <em>'.$edit_args['query_name'].'</em>',
    // content is the query_edit page
    'content' => theme('query_edit', $edit_args)
  );

  // add view link for pages
  if($row->type == 'page' && isset($row->path)){
    $admin_args['title'].= ' <a class="add-new-h2" target="_blank" href="'.get_bloginfo('wpurl').'/'.$row->path.'">View</a>';
  }

  // include the edit form
  print theme('admin_wrapper', $admin_args);
}

/*
 * Settings!
 */
function qw_save_settings($post){
  update_option('qw_edit_theme', $post['qw-theme']);
}
function qw_settings_page()
{
  qw_editors_init();
  $settings_args = array(
    'theme' => get_option('qw_edit_theme'),
    'edit_themes' => qw_all_editors(),
  );
  $args = array(
    'title' => 'Query Wrangler Settings',
    'content' => theme('query_settings', $settings_args)
  );

  print theme('admin_wrapper', $args);
}

/*
 * Create Query Page
 */
function qw_create_query() {
  $wizards = qw_all_wizards();

  $args = array(
    'title' => 'Create Query',
    'content' => theme('query_create', array('wizards' => $wizards))
  );

  print theme('admin_wrapper', $args);
}
/*
 * Export Query page
 */
function qw_export_page(){
  global $wpdb;
  $table = $wpdb->prefix.'query_wrangler';
  $row = $wpdb->get_row('SELECT name FROM '.$table.' WHERE id = '.$_GET['export']);

  $args = array(
    'title' => 'Export Query: <em>'.$row->name.'</em>',
    'content' => theme('query_export', array('query_id' => $_GET['export'])),
  );
  print theme('admin_wrapper', $args);
}
/*
 * Import Query Page
 */
function qw_import_page(){
  // show import page
  $args = array(
    'title' => 'Import Query',
    'content' => theme('query_import'),
  );
  print theme('admin_wrapper', $args);
}

/*
 * Docs Page
 */
function qw_docs_page(){
  // show import page
  $args = array(
    'title' => 'Documentation',
    'content' => theme('query_docs'),
  );
  print theme('admin_wrapper', $args);
}
