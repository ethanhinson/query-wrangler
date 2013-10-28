<?php
// hook into all wizards
add_filter('qw_wizards', 'qw_wizards_default');

function qw_wizards_default($wizards){
/*
  $wizards['default'] = array(
    'title' => 'Default',
    'description' => 'Very few settings at all.',
    'data_callback' => 'qw_wizard_default_data',
  );
*/
  $wizards['wp_default'] = array(
    'title' => 'WP_Query Default',
    'description' => 'All default settings for an empty WP_Query.<br />Like, $query = new WP_Query();',
    'data_callback' => 'qw_wizard_wp_default_data',
  );
  return $wizards;
}

/*
 * Default values for  new query
 *
 * @return array Default query settings
 *//*
function qw_wizard_default_data(){
  $query = array(
    'args' => array(
      'posts_per_page'  => 5,
      'offset'          => 0,
      'post_status'     => 'publish',
      'filters'         => array(),
      'sorts'           => array(),
    ),
    'display' => array(
      'style' => 'unformatted',
      'row_style' => 'posts',
      'post_settings' => array(
        'size' => 'complete',
      ),
    ),
  );

  return $query;
}
*/
function qw_wizard_wp_default_data(){
// post_type  = post
// post_status = publish
// sort date = DESC
  $query = array(
    'display' => array(
      'title' => '',
      'page'  => array(
        'pager' => array(
          'type'     => 'default',
          'previous' => '',
          'next'     => '',
        ),
        'path'  => '',
        'template-file' => 'index.php',
        'template-name' => 'Default',
      ),
      'style'  => 'unformatted',
      'header' => '',
      'footer' => '',
      'empty'  => '',
      'wrapper-classes' => '',
      'row_style'       => 'posts',
      'row_style_settings' => array (
        'posts' => array (
          'size' => 'complete',
        ),
        'fields' => array (
          'placeholder' => 'none',
        ),
      ),
      'post_settings'   => array(
        'size' => 'complete',
      ),
    ),
    'args' => array(
      'offset'         => '0',
      'post_status'    => 'publish',
      'posts_per_page' => '5',
      'sorts'          => array(
        'date' => array(
          'type'        => 'date',
          'hook_key'    => 'post_date',
          'name'        => 'date',
          'weight'      => '0',
          'order_value' => 'DESC',
        ),
      ),
      'filters' => array(
        'post_types' => array(
          'type'       => 'post_types',
          'hook_key'   => 'post_types',
          'name'       => 'post_types',
          'weight'     => '0',
          'post_types' => array(
            'post' => 'post',
          ),
          'exposed_label' => '',
          'exposed_desc' => '',
          'exposed_key' => '',
          'exposed_settings' => 
          array (
            'type' => 'select',
          ),          
        ),
      ),
    ),
  );
  return $query;
}