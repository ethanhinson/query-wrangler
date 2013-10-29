<?php
// hook into qw_all_filters()
add_filter('qw_filters', 'qw_filter_taxonomy_relation');

/*
 * Add filter to qw_filters
 */
function qw_filter_taxonomy_relation($filters){
  $filters['taxonomy_relation'] = array(
    'title' => 'Taxonomy Relation',
    'description' => 'Define how multiple taxonomy filters interact with each other.',
    'form_callback' => 'qw_filter_taxonomy_relation_form',
    'query_args_callback' => 'qw_filter_taxonomy_relation_args',
    // exposed
    //'exposed_form' => 'qw_filter_taxonomy_relation_exposed_form',
    //'exposed_process' => 'qw_filter_taxonomy_relation_exposed_process',
    //'exposed_settings_form_callback' => 'qw_filter_taxonomy_relation_exposed_settings_form',
  );
  return $filters;
}

/*
 * Convert values into query args
 */
function qw_filter_taxonomy_relation_args(&$args, $filter){
  if (isset($filter['values']['taxonomy_relation'])) {
    $args['tax_query']['relation'] = $filter['values']['taxonomy_relation'];
  }
}

/*
 * Filter form
 */
function qw_filter_taxonomy_relation_form($filter)
{ ?>
    <p>
      <select name="<?php print $filter['form_prefix']; ?>[taxonomy_relation]">
        <option value="AND" <?php if($filter['values']['taxonomy_relation'] == "AND") { print 'selected="selected"';} ?>>AND</option>
        <option value="OR" <?php if($filter['values']['taxonomy_relation'] == "OR") { print 'selected="selected"';} ?>>OR</option>
      </select>
    </p>
    <p class="description">How do multiple taxonomy filters relate to each other?  <br />
      AND requires posts to contain at least one term from each taxonomy filter.  OR allows posts to contain any terms from all of the taxonomy filters.
    </p>    
  <?php
}

/*
 * Process submitted exposed form values
 */
function qw_filter_taxonomy_relation_exposed_process(&$args, $filter, $values){
}

/*
 * Exposed form
 */
function qw_filter_taxonomy_relation_exposed_form($filter, $values)
{

}