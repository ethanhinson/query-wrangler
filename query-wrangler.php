<?php
/*

******************************************************************

Contributors:      daggerhart, forrest.livengood
Plugin Name:       Query Wrangler
Plugin URI:        http://www.widgetwrangler.com/query-wrangler
Description:       Query Wrangler provides an intuitive interface for creating complex WP queries as pages or widgets. Based on Drupal Views.
Author:            Jonathan Daggerhart, Forrest Livengood
Author URI:        http://www.websmiths.co
Version:           2.0beta1

******************************************************************

Copyright 2010  Jonathan Daggerhart  (email : jonathan@daggerhart.com)
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License, version 2, as
published by the Free Software Foundation.
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

******************************************************************

*/

// some useful definitions
define('QW_VERSION', 2.0);
define('QW_PLUGIN_DIR', dirname(__FILE__));
define('QW_PLUGIN_URL', plugins_url( '', __FILE__ ));
define('QW_DEFAULT_THEME', 'views');

// Query Widget
include_once QW_PLUGIN_DIR.'/widget.query.php';

if (!is_admin()){
  add_action('init', 'qw_init');
}
add_action('admin_init', 'qw_init', 900);
add_action('admin_init', 'qw_check_version', 901);
add_action('admin_head', 'qw_admin_css');

// add menu very last so we don't get replaced by another menu item
add_action( 'admin_menu', 'qw_menu', 9999);
add_shortcode('query','qw_single_query_shortcode');

// include files hook
add_action('qw_includes', 'qw_includes_default', 0);
function qw_includes_default($includes){
  // Necessary functions to show a query
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/query.inc';
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/theme.inc';
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/query-pages.inc';
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/handlers.inc';
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/exposed.inc';

  // basic settings
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/basics/display_title.inc';
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/basics/display_types.inc';
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/basics/template_styles.inc';
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/basics/row_styles.inc';
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/basics/posts_per_page.inc';
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/basics/offset.inc';
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/basics/pagers.inc';
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/basics/post_status.inc';
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/basics/header.inc';
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/basics/footer.inc';
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/basics/empty.inc';
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/basics/wrapper_settings.inc';
  //$includes['frontend'][] = QW_PLUGIN_DIR.'/includes/basics/page_path.inc'; // disabled
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/basics/page_template.inc';

  // fields
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/fields/author.inc';
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/fields/author_avatar.inc';
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/fields/fields.inc';
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/fields/file_attachment.inc';
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/fields/image_attachment.inc';
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/fields/meta_value.inc';

  // filters
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/filters/meta_key.inc';
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/filters/meta_key_value.inc';
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/filters/meta_value.inc';
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/filters/post_id.inc';
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/filters/post_parent.inc';
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/filters/post_types.inc';
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/filters/taxonomies.inc';

  // contextual_filters (disabled for now)
  //$includes['frontend'][] = QW_PLUGIN_DIR.'/includes/contextual_filters/contexts.inc';
  //$includes['frontend'][] = QW_PLUGIN_DIR.'/includes/contextual_filters/taxonomies.inc';

  // sort options
  $includes['frontend'][] = QW_PLUGIN_DIR.'/includes/sorts/sorts.inc';

  // example
  $includes['frontend'][] = QW_PLUGIN_DIR.'/docs/docs.php';

  // admin
  $includes['admin'][] = QW_PLUGIN_DIR.'/admin/admin-theme.inc';
  $includes['admin'][] = QW_PLUGIN_DIR.'/admin/admin-query.inc';
  $includes['admin'][] = QW_PLUGIN_DIR.'/admin/admin-pages.inc';
  $includes['admin'][] = QW_PLUGIN_DIR.'/admin/admin-ajax.inc';
  $includes['admin'][] = QW_PLUGIN_DIR.'/admin/admin-editors.inc';
  $includes['admin'][] = QW_PLUGIN_DIR.'/admin/admin-wizards.inc';
  return $includes;
}

/*
 * Init handler
 */
function qw_init(){
$_SESSION['qw']['time']['load']['start'] = microtime(1);
  qw_init_frontend();

  // admin only
  if(is_admin()){
    qw_init_admin();
  }
$_SESSION['qw']['time']['load']['end'] = microtime(1);
}

/*
 * Init frontend
 */
function qw_init_frontend(){
  // include Template Wrangler
  if(!function_exists('theme')){
    include_once QW_PLUGIN_DIR.'/template-wrangler.inc';
  }
  // Wordpress hooks
  include_once QW_PLUGIN_DIR.'/includes/hooks.inc';

  // include all frontend files
  $includes = qw_all_includes();
  foreach($includes['frontend'] as $include){
    if (file_exists($include)){
      include_once $include;
    }
  }
}

/*
 * Init admin
 */
function qw_init_admin(){
  // include all admin files
  $includes = qw_all_includes();
  foreach($includes['admin'] as $include){
    if (file_exists($include)){
      include_once $include;
    }
  }

  // load JS appropriately
  add_action( 'wp_ajax_nopriv_qw_form_ajax', 'qw_form_ajax' );
  add_action( 'wp_ajax_qw_form_ajax', 'qw_form_ajax' );

  // js
  if (isset($_GET['page'])){
    if($_GET['page'] == 'query-wrangler'){
      // edit page & not on export page
      if(!empty($_GET['edit']) &&
         empty($_GET['export']))  {
        add_action( 'admin_enqueue_scripts', 'qw_admin_js' );
        qw_editors_init();
      }

      // list page
      if(empty($_GET['edit'])){
        add_action( 'admin_enqueue_scripts', 'qw_admin_list_js' );
      }
    }

    if($_GET['page'] == 'qw-create'){
      add_action( 'admin_enqueue_scripts', 'qw_admin_create_js' );
    }
  }
}

/*
 * All my hook_menu implementations
 */
function qw_menu()
{
  global $menu;
  // get the first available menu placement around 30 abitrarily
  $menu_placement = 1000;
  for($i=30;$i<100;$i++){
    if(!isset($menu[$i])){ $menu_placement = $i; break; }
  }
  // http://codex.wordpress.org/Function_Reference/add_menu_page
  $list_page    = add_menu_page( 'Query Wrangler', 'Query Wrangler', 'manage_options', 'query-wrangler', 'qw_page_handler', '', $menu_placement);
  // http://codex.wordpress.org/Function_Reference/add_submenu_page
  $create_page  = add_submenu_page( 'query-wrangler', 'Create New Query', 'Add New', 'manage_options', 'qw-create', 'qw_create_query');
  $import_page  = add_submenu_page( 'query-wrangler', 'Import', 'Import', 'manage_options', 'qw-import', 'qw_import_page');
  $settings_page= add_submenu_page( 'query-wrangler', 'Settings', 'Settings', 'manage_options', 'qw-settings', 'qw_settings_page');
  $docs_page    = add_submenu_page( 'query-wrangler', 'Docs', 'Docs', 'manage_options', 'qw-docs', 'qw_docs_page');
  //$debug_page  = add_submenu_page( 'query-wrangler', 'Debug', 'Debug', 'manage_options', 'qw-debug', 'qw_debug');
}

/*
 * Shortcode support for all queries
 */
function qw_single_query_shortcode($atts) {
  $short_array = shortcode_atts(array('id' => ''), $atts);
  extract($short_array);

  // get the query options
  $options = qw_generate_query_options($id);

  // get formatted query arguments
  $args = qw_generate_query_args($options);

  // set the new query
  $wp_query = new WP_Query($args);

  // get the themed content
  $themed = qw_template_query($wp_query, $options);
  // reset because worpress hates programmers
  wp_reset_postdata();
  return $themed;
}

/*===================================== DB TABLES =========================================*/
/*
 * Activation hooks for database tables
 */
function qw_query_wrangler_table(){
  global $wpdb;
  $table_name = $wpdb->prefix."query_wrangler";
  $sql = "CREATE TABLE " . $table_name . " (
	  id mediumint(9) NOT NULL AUTO_INCREMENT,
   name varchar(255) NOT NULL,
   slug varchar(255) NOT NULL,
   type varchar(16) NOT NULL,
   path varchar(255),
	  data text NOT NULL,
	  UNIQUE KEY id (id)
	);";

  require_once(ABSPATH . 'wp-admin/includes/upgrade.php');
  dbDelta($sql);
}
register_activation_hook(__FILE__,'qw_query_wrangler_table');

/*
 * Override terms table
 */
function qw_query_overrides_table(){
  global $wpdb;
  $table_name = $wpdb->prefix."query_overrides";
  $sql = "CREATE TABLE " . $table_name . " (
	  query_id mediumint(9) NOT NULL,
   override_hook varchar(24) NOT NULL,
   override_id varchar(24) NOT NULL,
  KEY `query_id` (`query_id`)
	);";

  require_once(ABSPATH . 'wp-admin/includes/upgrade.php');
  dbDelta($sql);
}
register_activation_hook(__FILE__,'qw_query_overrides_table');

/*
 * See the output time for a query
 */
function qw_debug_query_time(){
  $output = '';
  if (is_array($_SESSION['qw']['time'])){
    $time = $_SESSION['qw']['time'];

    foreach ($time as $k => $v){
      $math = round($v['end'] - $v['start'], 4);
      $output.= "<div>".$k." took ".$math."s</div>";
    }
  }

  return $output;
}