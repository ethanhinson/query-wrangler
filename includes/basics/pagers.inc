<?php
// add default pager types
add_filter('qw_pager_types', 'qw_pager_types_default', 0);
// hook into qw_basics
add_filter('qw_basics', 'qw_basic_settings_pager');

/*
 * Basic Settings
 */
function qw_basic_settings_pager($basics){
  $basics['pager'] = array(
    'title' => 'Pager',
    'type' => 'page',
    'description' => 'Enable pagination for this query.',
    'form_callback' => 'qw_basic_settings_pager_form',
    'weight' => 0,
  );
  return $basics;
}

/*
 * Setup pager types
 */
function qw_pager_types_default($pagers) {
  $pagers['default'] = array(
    'title' => 'Default',
    'callback' => 'qw_theme_pager_default'
  );
  $pagers['numbers']	= array(
    'title' => 'Page Numbers',
    'callback' => 'qw_theme_pager_numbers'
  );

  // WP PageNavi Plugin
  if(function_exists('wp_pagenavi')) {
    $pagers['pagenavi'] = array(
      'title' => 'PageNavi',
      'callback' => 'wp_pagenavi'
    );
  }
  return $pagers;
}

/*
 * Form callback
 */
function qw_basic_settings_pager_form($basic, $options)
{
  $pager = $options['display']['page']['pager'];
  $pager_types = qw_all_pager_types();
  $use_pager = (isset($pager['active'])) ? 'checked="checked"': '';
  ?>
  <div class="qw-setting">
    <label class="qw-label">Use Pagination:</label>
    <label class='qw-field-checkbox'>
      <input type='checkbox'
             name="<?php print $basic['form_prefix']; ?>[pager][active]"
             <?php print $use_pager;?> />
    </label>
    <div class="clear-left">
      <p class="description"><?php print $basic['description']; ?></p>
    </div>
  </div>

  <div class="qw-setting">
    <label class="qw-label">Pager Type:</label>
    <select name="<?php print $basic['form_prefix']; ?>[pager][type]">
      <?php
        foreach($pager_types as $pager_name => $pager_options)
        {
          $selected = ($pager['type'] == $pager_name) ? 'selected="selected"' : '';
          ?>
          <option value="<?php echo $pager_name; ?>"
                  <?php echo $selected; ?>>
            <?php echo $pager_options['title']; ?>
          </option>
          <?php
        }
      ?>
    </select>
    <p class="description">Select which type of pager to use.</p>
  </div>

  <div>
    <label class="qw-label">Previous Label:</label>
    <input type="text"
           name="<?php print $basic['form_prefix']; ?>[pager][previous]"
           value="<?php print $pager['previous']; ?>" />
  </div>
  <div>
    <label class="qw-label">Next Label:</label>
    <input type="text"
           name="<?php print $basic['form_prefix']; ?>[pager][next]"
           value="<?php print $pager['next']; ?>" />
  </div>
  <p class="description">Use the following options to change the Default Pager labels.</p>
  <?php
}

/*
 * Custom Default Pager
 *
 * @param array $pager Query options for pager
 * @param object $wp_query Object
 */
function qw_theme_pager_default($pager, &$wp_query)
{
  // help figure out the current page
  $path_array = explode('/page/', $_SERVER['REQUEST_URI']);

  $pager_themed = '';
  $pager['next'] = ($pager['next']) ? $pager['next'] : 'Next Page &raquo;';
  $pager['previous'] = ($pager['previous']) ? $pager['previous'] : '&laquo; Previous Page';

  if($page = qw_get_page_number($wp_query))
  {
    $wpurl = get_bloginfo('wpurl');

    // previous link with page number
    if($page >= 3){
      $pager_themed.= '<div class="query-prevpage">
                        <a href="'.$wpurl.$path_array[0].'/page/'.($page-1).'">'.$pager['previous'].'</a>
                      </div>';
    }
    // previous link with no page number
    else if ($page == 2)
    {
      $pager_themed.= '<div class="query-prevpage">
                        <a href="'.$wpurl.$path_array[0].'">'.$pager['previous'].'</a>
                      </div>';
    }

    // next link
    if(($page+1) <= $wp_query->max_num_pages){
      $pager_themed.= '<div class="query-nextpage">
                        <a href="'.$wpurl.$path_array[0].'/page/'.($page+1).'">'.$pager['next'].'</a>
                      </div>';
    }

    return $pager_themed;
  }
}

/*
 * Default Pager with page numbers
 *
 * @param array $pager Query options for pager
 * @param object $wp_query Object
 *
 * @return string HTML for pager
 */
function qw_theme_pager_numbers($pager, $wp_query)
{
		$big = intval($wp_query->found_posts.'000');
		$args = array(
    'base' => str_replace( $big, '%#%', get_pagenum_link( $big ) ),
    'format' => '?paged=%#%',
    'current' => max( 1, get_query_var('paged') ),
    'total' => $wp_query->max_num_pages
		);
  $pager_themed = paginate_links($args);
  return $pager_themed;
}
