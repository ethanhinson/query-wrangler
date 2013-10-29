<?php
/*
 * Create the new Query
 *
 * @param $_POST data
 * @return int New Query ID
 */
function qw_insert_new_query($submitted){
  global $wpdb;
  $table_name = $wpdb->prefix."query_wrangler";

  // wizard data
  $wizards = qw_all_wizards();
  if (isset($wizards[$submitted['qw-data-wizard']]) &&
      function_exists($wizards[$submitted['qw-data-wizard']]['data_callback']))
  {
    $wizard = $wizards[$submitted['qw-data-wizard']];
    $data = $wizard['data_callback']();
  }
  else {
    $data = qw_wizard_default_data();
  }

  $type = 'default';
  if (is_array($submitted['qw-display']['types'])){
    $data['display']['types'] = $submitted['qw-display']['types'];
    $type = implode(',',$submitted['qw-display']['types']);
  }

  $values = array(
    'name' => $submitted['qw-name'],
    'slug' => qw_make_slug($submitted['qw-name']),
    'type' => $type,
    'path' => ($submitted['page-path']) ? urlencode($submitted['page-path']) : NULL,
    'data' => qw_serialize($data),
  );

  $wpdb->insert($table_name, $values);
  return $wpdb->insert_id;
}

/*
 * Update existing query
 *
 * @param $_POST data
 */
function qw_update_query($submitted){
  global $wpdb;
  $options = $submitted['qw-query-options'];
  $handlers = qw_preprocess_handlers($options);

  // if you can't tell, i'm having a lot of trouble with slashes
  $submitted = array_map( 'stripslashes_deep', $submitted );

  // handle page settings
  if(isset($options['display']['page']['template-file']))
  {
    // handle template name
    if($options['display']['page']['template-file'] == 'index.php'){
      $options['display']['page']['template-name'] = 'Default';
    }
    else {
      $page_templates = get_page_templates();
      foreach($page_templates as $name => $file){
        if($options['display']['page']['template-file'] == $file){
          $options['display']['page']['template-name'] = $name;
        }
      }
    }
  }

  // hook for presave
  do_action_ref_array('qw_pre_save', array(&$options));
  $type = 'default';
  if (is_array($options['display']['types'])){
    $type = implode(',',$options['display']['types']);
  }

  $page_path = '';
  // update for pages
  if($options['display']['page']['path']){
    $page_path = ($options['display']['page']['path']) ? $options['display']['page']['path'] : '';

    // handle opening slash
    // checking against $wp_query->query['pagename'], so, no slash
    if(substr($page_path, 0, 1) == '/'){
      $page_path = ltrim($page_path, '/');
    }
  }

  $query_id = $submitted['query-id'];
  $table_name = $wpdb->prefix."query_wrangler";
  $data = array(
    'type' => $type,
    'path' => $page_path,
    'data' => qw_serialize($options),
  );
  $where = array(
    'id' => $query_id,
  );
  $wpdb->update($table_name, $data, $where);

  // additional override work
  // allow contextual filters change to save values to the db
  if(is_array($handlers['contextual_filter']['items']))
  {
    foreach($handlers['contextual_filter']['items'] as $name => $item)
    {
      if ($item['can_override'] &&
          $item['values']['do_override'] &&
          function_exists($item['query_update_callback']))
      {
        // give the query_id
        $item['query_id'] = $query_id;
        $item['query_update_callback']($item);
      }
    }
  }

  // send back to edit page
  wp_redirect(get_bloginfo('wpurl').'/wp-admin/admin.php?page=query-wrangler&edit='.$query_id);
}
/*
 * Delete an existing query
 *
 * @param query id
 */
function qw_delete_query($query_id){
  global $wpdb;
  $table_name = $wpdb->prefix."query_wrangler";
  $sql = "DELETE FROM ".$table_name." WHERE id = ".$query_id;
  $wpdb->query($sql);
}

/*
 * Export a query into code
 * @param
 *   $query_id - the query's id number
 */
function qw_query_export($query_id){
  global $wpdb;
  $table_name = $wpdb->prefix."query_wrangler";
  $sql = "SELECT id,name,slug,type,path,data FROM ".$table_name." WHERE id = ".$query_id;

  $row = $wpdb->get_row($sql, ARRAY_A);
  unset($row['id']);
  // unserealize the stored data
  $row['data'] = qw_unserialize($row['data']);
  $export = var_export($row,1);

  return "\$query = ".$export.";";
}

function qw_query_clean_export($string){
  $search = array(
    // find => replace
    "=> \n  " => "=> ",
    "array (" => "array("
  );
  return str_replace(array_keys($search), array_values($search), $string);
}

/*
 * Import a query into the database
 *
 */
function qw_query_import($submitted){
  global $wpdb;
  $table = $wpdb->prefix."query_wrangler";

  eval(stripslashes($submitted['import-query']));

  if($submitted['import-name']){
    $query['name'] = $submitted['import-name'];
    $query['slug'] = qw_make_slug($submitted['import-name']);
  }
  $query['data'] = qw_serialize($query['data']);
  $wpdb->insert($table, $query);
  return $wpdb->insert_id;
}
/*
 * Handle updating override ids for contextual filters
 */
function qw_update_override_values($handler, $new_values = array()){
  global $wpdb;
  $table = $wpdb->prefix."query_overrides";
  $query_id = $handler['query_id'];

  // select current existing overridden categories for this query.
  $sql = "SELECT
            override_id
          FROM ".$table."
          WHERE query_id = ".$query_id."
            AND override_hook = '".$handler['hook_key']."'";

  $rows = $wpdb->get_results($sql);

  // delete all old ones
  foreach ($rows as $row){
    $wpdb->query('DELETE FROM '.$table.'
                  WHERE query_id = '.$query_id.'
                    AND override_hook = "'.$handler['hook_key'].'"
                    AND override_id = '.$row->override_id);
  }

  // add new ones
  foreach ($new_values as $id){
    $data = array(
      'query_id' => $query_id,
      'override_hook' => $handler['hook_key'],
      'override_id' => $id,
    );
    $wpdb->insert($table, $data);
  }
}

/*
 * Slug creation
 */
function qw_make_slug($string){
  $search = array("!","@","#","$","%","^","&","*","(",")","-","+","=","{","}","[","]","\\","|",":",";","'","<",",",">",".","?","/","~","`");
  return str_replace(" ", "_", strtolower(str_replace($search, "", strip_tags($string))));
}

/*
 * Custom for outputting text/html into a textarea
 */
function qw_textarea($value){
  return esc_textarea(str_replace("\\", "", $value));
}

/*
 * Checking current version of plugin to handle upgrades
 */
function qw_check_version()
{
  if ($last_version = get_option('qw_plugin_version')) {
    // compare versions
    if ($last_version < QW_VERSION){
      // include upgrade inc
      include_once QW_PLUGIN_DIR.'/upgrade.php';
      $upgrade_function = 'qw_upgrade_'.qw_make_slug($last_version).'_to_'.qw_make_slug(QW_VERSION);
      $upgrade_to_current = 'qw_upgrade_'.qw_make_slug($last_version).'_to_current';

      if(function_exists($upgrade_function)){
        $upgrade_function();
      } else if(function_exists($upgrade_to_current)) {
        $upgrade_to_current();
      }
      update_option('qw_plugin_version', QW_VERSION);
    }
  }
  else {
    // first upgrade
    include QW_PLUGIN_DIR.'/upgrade.php';
    qw_upgrade_12_to_13();
    // set our version numer
    update_option('qw_plugin_version', QW_VERSION);
  }
}
