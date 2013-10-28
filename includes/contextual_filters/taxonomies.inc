<?php
// hook into qw_all_contextual_filters()
add_filter('qw_contextual_filters', 'qw_contextual_filter_taxonomies');

/*
 * Add filter to qw_contextual_filters
 */
function qw_contextual_filter_taxonomies($contextual_filters){
  $ts = get_taxonomies(array('_builtin' => false), 'objects');
  $builtin = get_taxonomies(array('_builtin' => true), 'objects');
  $ts['category'] = $builtin['category'];
  $ts['post_tag'] = $builtin['post_tag'];

  if(count($ts) > 0){
    foreach ($ts as $t){
      $contextual_filters['taxonomy_'.$t->name] = array(
        'title' => 'Taxonomy: '. $t->label,
        'description' => 'Select which taxonomies to accept contexts from, and how to treat those taxonomies.',
        'taxonomy' => $t,
        'form_callback' => 'qw_contextual_filter_taxonomies_form',
        // allow this contextual_filter to override pages
        'can_override' => true,
        'override_form_callback' => 'qw_contextual_filter_taxonomies_override_form',
        'query_args_callback' => 'qw_contextual_filter_taxonomies_args',
        // additional update callback for saving a query
        'query_update_callback' => 'qw_contextual_filter_taxonomies_query_update',
        // return the appropriate override_id
        'query_override_id_callback' => 'qw_contextual_filter_taxonomies_override_id',
      );

      // update titles for built in taxonomies
      if ($t->_builtin){
        $contextual_filters['taxonomy_'.$t->name]['title'] = 'Taxonomy: '.$t->label;
      }
    }
  }
  return $contextual_filters;
}

/*
 * Convert contextual values into query args
 */
function qw_contextual_filter_taxonomies_args(&$args, $contextual_filter){
  if (!isset($args['tax_query']) || !is_array($args['tax_query'])){
    $args['tax_query'] = array();
  }

  if (isset($contextual_filter['values']['context']))
  {
    // this is the query-wrangler overriden wp_query
    global $wp_query;

    // original wp_query before QW got involved
    $original_query = $wp_query->original_wp_query;

    // get terms from context
    switch($contextual_filter['values']['context'])
    {
      case 'single':
        // look for single post data
        if ($post = $wp_query->post){
          $terms = get_the_terms($post->ID, $contextual_filter['taxonomy']->name);

          // build values array like filter expects it
          if (is_array($terms)){
            $values = array();
            foreach ($terms as $term){
              $values[$term->term_id] = $term->slug;
            }
          }
        }
        break;

      // get terms from global wp_query
      case 'global_query':
        $term_slug = $original_query->query_vars[$contextual_filter['taxonomy']->query_var];

        if ($term = qw_get_term($term_slug, 'slug')){
          $values = array();
          $values[$term->term_id] = $term->slug;
        }
        break;
    }

    // if we have values, create a new filter for those values
    if (is_array($values)){
      // create the new filter
      $filter = $contextual_filter;
      $filter['values']['terms'] = $values;
      $filter['values']['operator'] = $contextual_filter['values']['operator'];

      // leverage existing taxonomy query_args_callback to create the filter
      qw_filter_taxonomies_args($args, $filter);
    }
  }
}

/*
 * Return the appropriate override_id by looking for query_vars
 */
function qw_contextual_filter_taxonomies_override_id($contextual_filter){
  $term = get_query_var($contextual_filter['taxonomy']->query_var);
  // make sure we only have 1
  if(is_array($term)){
     $term = array_pop($term);
  }
  $term = qw_get_term($term, 'slug');
  if(isset($term->term_id)){
    return $term->term_id;
  }
  return false;
}
/*
 *
 */
function qw_contextual_filter_taxonomies_query_update($contextual_filter){
  // save items to db table if not exists.
  if (isset($contextual_filter['values']['terms']) && is_array($contextual_filter['values']['terms'])){
    $new_values = array_keys($contextual_filter['values']['terms']);
    qw_update_override_values($contextual_filter, $new_values);
  }
}

/*
 * Show tax query operator on form
 */
function qw_contextual_filter_taxonomies_form($contextual_filter)
{
  // leverage the filter's forms
  qw_filter_taxonomies_form_operator($contextual_filter);
  qw_filter_taxonomies_form_include_children($contextual_filter);
}

/*
 * Override Filter form
 */
function qw_contextual_filter_taxonomies_override_form($contextual_filter)
{
  $terms = get_terms($contextual_filter['taxonomy']->name, array('hide_empty' => false));
  // leverage the filter's forms
  qw_filter_taxonomies_form_terms_checkboxes($contextual_filter, $terms);
}