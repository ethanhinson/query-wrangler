<?php
// hook into qw_all_filters()
add_filter('qw_filters', 'qw_filter_meta_value');

/*
 * Add filter to qw_filters
 */
function qw_filter_meta_value($filters){
  $filters['meta_value'] = array(
    'title' => 'Meta Value',
    'description' => 'Filter for a specific meta_value.',
    'form_callback' => 'qw_filter_meta_value_form',
    'query_args_callback' => 'qw_filter_meta_value_args',
    // exposed
    'exposed_form' => 'qw_filter_meta_value_exposed_form',
    'exposed_process' => 'qw_filter_meta_value_exposed_process',
  );
  return $filters;
}

/*
 * Convert values into query args
 */
function qw_filter_meta_value_args(&$args, $filter){
  $args['meta_value'] = stripslashes($filter['values']['meta_value']);
}

/*
 * Filter form
 */
function qw_filter_meta_value_form($filter)
{ ?>
    <textarea name="<?php print $filter['form_prefix']; ?>[meta_value]"
              class="qw-meta-value"><?php print stripcslashes($filter['values']['meta_value']); ?></textarea>
  <?php
}

/*
 * Process submitted exposed form values
 */
function qw_filter_meta_value_exposed_process(&$args, $filter, &$values){
  // default values if submitted is empty
  qw_filter_meta_value_exposed_default_values($filter, $values);

  // check allowed values
  if (isset($filter['values']['exposed_limit_values'])){
    if ($values == $filter['values']['meta_value']){
      $args['meta_value'] = $values;
    }
  }
  else {
    $args['meta_value'] = $values;
  }
}

/*
 * Exposed form
 */
function qw_filter_meta_value_exposed_form($filter, &$values)
{
  // default values
  qw_filter_meta_value_exposed_default_values($filter, $values);
  ?>
  <input type="text"
         name="<?php print $filter['exposed_key']; ?>"
         value="<?php print $values; ?>" />
  <?php
}
//function qw_filter_meta_value_exposed_settings_form($filter){}
/*
 * Simple helper funtion to handle default values
 */
function qw_filter_meta_value_exposed_default_values($filter, &$values){
  if (isset($filter['values']['exposed_default_values'])){
    if (is_null($values)){
      $values = $filter['values']['meta_value'];
    }
  }
}