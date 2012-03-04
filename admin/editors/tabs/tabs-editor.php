<?php
/*
 * Where do all these variables come from?
 * They are coming from the arguments sent along with the theme('query_edit', $args) function in query-wrangler.php
 *
 * All keys in the argument array become variables in the template file
 *
 * See the following link for more details on how that works:
 * https://github.com/daggerhart/Query-Wrangler/wiki/Template-Wrangler
 */
?>
<form id="qw-edit-query-form" action='admin.php?page=query-wrangler&action=update&edit=<?php print $query_id; ?>&noheader=true' method='post'>
  <div id="qw-query-action-buttons">
    <div id="query-actions">
      <a href="admin.php?page=query-wrangler&export=<?php print $query_id; ?>">Export</a>
    </div>
    <input class='button-primary' type="submit" name="save" value="Save" />
    <input type="hidden" name="query-id" value="<?php print $query_id; ?>" />
  </div>

  <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>


  <div id="qw-query-admin-options-wrap">

    <ul>
      <!--<li><a href="#tabs-basics">Basic Settings</a></li>-->
      <li><a href="#tabs-args">Query Settings</a></li>
      <li><a href="#tabs-display">Display Settings</a></li>
      <?php if ($query_type == "page" || $query_type == "override") { ?>
        <li><a href="#tabs-page">Page Settings</a></li>
      <?php } ?>
      <?php if ($query_type == "override") { ?>
        <li><a href="#tabs-override">Override Settings</a></li>
      <?php } ?>
      <li><a href="#tabs-fields">Fields</a></li>
      <li><a href="#tabs-filters">Filters</a></li>
      <li><a href="#tabs-sorts">Sorts</a></li>
    </ul>

  <!-- basic settings 'args' -->
    <div id="tabs-args" class="qw-query-admin-tabs">
      <?php
        foreach($basics as $basic)
        {
          if ($basic['type'] == 'args' &&
              in_array($query_type, $basic['allowed_query_types']))
          { ?>
            <div class="qw-setting">
              <label class="qw-label"><?php print $basic['title']; ?>:</label>
              <?php
                if(isset($basic['form']))
                { ?>
                  <div class="qw-form">
                    <?php print $basic['form']; ?>
                  </div>
                  <?php
                }
              ?>
            </div>
            <?php
          }
        }
      ?>
    </div>

  <!-- basic settings 'display' -->
    <div id="tabs-display" class="qw-query-admin-tabs">
      <?php
        foreach($basics as $basic)
        {
          if ($basic['type'] == 'display' &&
              in_array($query_type, $basic['allowed_query_types']))
          { ?>
            <div class="qw-setting">
              <label class="qw-label"><?php print $basic['title']; ?>:</label>
              <?php
                if(isset($basic['form']))
                { ?>
                  <div class="qw-form">
                    <?php print $basic['form']; ?>
                  </div>
                  <?php
                }
              ?>
            </div>
            <?php
          }
        }
      ?>
    </div>

<!-- Page Settings -->
    <div id="tabs-page" class="qw-query-admin-tabs">
      <?php
        foreach($basics as $basic)
        {
          if ($basic['type'] == 'page' &&
              in_array($query_type, $basic['allowed_query_types']))
          { ?>
            <div class="qw-setting">
              <?php
                // hack to prettify
                if ($basic['title'] != 'Pager')
                { ?>
                  <label class="qw-label"><?php print $basic['title']; ?>:</label>
                  <?php
                }

                if(isset($basic['form']))
                { ?>
                  <div class="qw-form">
                    <?php print $basic['form']; ?>
                  </div>
                  <?php
                }
              ?>
            </div>
            <?php
          }
        }
      ?>
    </div>

<!-- Overrides -->
  <?php
    // override queries have different category and tag options
    if($query_type == "override")
    { ?>
      <div id="tabs-override" class="qw-query-admin-tabs">
        <!-- override categories -->
        <div id="qw-override-categories" class="qw-setting">
          <label class="qw-label">Category Pages:</label>
          <p class="description">Select which categories to override.</p>
          <div class="qw-checkboxes">
            <?php
              // List all categories as checkboxes
              foreach($category_ids as $cat_id)
              {
                $cat_name = get_cat_name($cat_id);
                $cat_checked = (isset($options['override']['cats'][$cat_id])) ? 'checked="checked"' : '';
                ?>
                <label class="qw-query-checkbox">
                  <input type="checkbox"
                         name="qw-query-options[override][cats][<?php print $cat_id; ?>]"
                         value="<?php print $cat_name; ?>"
                         <?php print $cat_checked; ?> />
                  <?php print $cat_name; ?>
                </label>
                <?php
              }
            ?>
          </div>
        </div>
        <!-- override tags -->
        <div id="qw-override-tags" class="qw-setting">
          <label class="qw-label">Tag Pages:</label>
          <p class="description">Select which tags to override.</p>
          <div class="qw-checkboxes">
            <?php
              foreach($tags as $tag)
              {
                $tag_checked = (isset($options['override']['tags'][$tag->term_id])) ? 'checked="checked"' : '';
                ?>
                <label class="qw-query-checkbox">
                  <input type="checkbox"
                         name="qw-query-options[override][tags][<?php print $tag->term_id; ?>]"
                         value="<?php print $tag->name; ?>"
                         <?php print $tag_checked; ?> />
                  <?php print $tag->name; ?>
                </label>
                <?php
              }
            ?>
          </div>
        </div>
      </div>
      <?php
    }
  ?>

<!-- fields -->
    <div id="tabs-fields" class="qw-query-admin-tabs">
      <div class="qw-query-add-titles qw-setting">
        <span class="qw-query-title button" title="qw-display-add-fields">
          Add Fields
        </span>
      </div>
      <p class="description">Click to change settings.  Drag and drop the fields to change their order.</p>

      <!-- edit fields -->
      <div id="existing-fields" class="qw-sortable-list">
        <?php
          if(is_array($fields) && count($fields) > 0)
          {
            $tokens = array();
            // loop through existing fields
            foreach($fields as $field)
            {
              $tokens[$field['name']] = '{{'.$field['name'].'}}';
              $args = array(
                'image_sizes' => $image_sizes,
                'file_styles' => $file_styles,
                'field' => $field,
                'weight' => $field['weight'],
                'options' => $options,
                'display' => $display,
                'args'  => $args,
                'tokens' => $tokens,
              );
              print theme('query_field', $args);
            }
          }
          else
          { ?>
            <div class="qw-empty-list ui-state-highlight ui-corner-all">
              No fields yet.  Click 'Add Fields' at the top to begin.
            </div>
            <?php
          }
        ?>
      </div>
      <!-- /edit fields -->

    </div>

<!-- sorts -->
    <div id="tabs-sorts" class="qw-query-admin-tabs">

      <div class="qw-query-add-titles qw-setting">
        <span class="qw-query-title button" title="qw-display-add-sorts">
          Add Sort Options
        </span>
      </div>
      <p class="description">Click to change settings.  Drag and drop the sort options to change their order.</p>

      <!-- edit sorts -->
      <div id="existing-sorts" class="qw-sortable-list">
        <?php
          if(is_array($sorts) && count($sorts) > 0)
          {
            // loop through existing sorts
            foreach($sorts as $sort_name => $sort)
            {
              $args = array(
                'sort' => $sort,
                'weight' => $sort['weight'],
              );
              print theme('query_sort', $args);
            }
          }
        ?>
        <div class="qw-empty-list ui-state-highlight ui-corner-all">
          No sort options yet.  Click 'Add Sort Options' at the top to change the order of the results.
        </div>
      </div>
      <!-- /edit sorts -->
    </div>

<!-- filters -->
    <div id="tabs-filters" class="qw-query-admin-tabs">

      <div class="qw-query-add-titles qw-setting">
        <span class="qw-query-title button" title="qw-display-add-filters">
          Add Filters
        </span>
      </div>
      <p class="description">Click to change settings.  Drag and drop the filters to change their order.</p>
      <!-- edit Filters -->
      <div id="existing-filters" class="qw-sortable-list">
        <?php
          if(is_array($filters) && count($filters) > 0)
          {
            // loop through existing filters
            foreach($filters as $filter_name => $filter)
            {
              $args = array(
                'filter' => $filter,
                'weight' => $filter['weight'],
              );
              print theme('query_filter', $args);
            }
          }
        ?>
        <div class="qw-empty-list ui-state-highlight ui-corner-all">
          No filters yet.  Click 'Add Filters' at the top to begin filtering.
        </div>
      </div>
      <!-- /edit filters -->
    </div>
  </div>


<!-- Add Handlers -->
        <!-- add sorts -->
        <div id="qw-display-add-sorts" class="qw-hidden">
          <input class="add-handler-type" type="hidden" value="sort">
          <p class="description">Select options for sorting the query results.</p>
          <div class="qw-checkboxes">
            <?php
              // loop through sorts
              foreach($all_sorts as $hook_key => $sort)
              {
                ?>
                <label class="qw-sort-checkbox">
                  <input type="checkbox"
                         value="<?php print $sort['type']; ?>" />
                  <input class="qw-hander-hook_key"
                         type="hidden"
                         value="<?php print $sort['hook_key']; ?>" />
                  <?php print $sort['title']; ?>
                </label>
                <p class="description qw-desc"><?php print $sort['description']; ?></p>
                <?php
              }
            ?>
          </div>
        </div>

        <!-- add fields -->
        <div id="qw-display-add-fields" class="qw-hidden">
          <input class="add-handler-type" type="hidden" value="field">
          <p class="description">Select Fields to add to this query's output.</p>
          <div class="qw-checkboxes">
            <?php
              // loop through fields
              foreach($all_fields as $hook_key => $field)
              {
                ?>
                <label class="qw-field-checkbox">
                  <input type="checkbox"
                         value="<?php print $field['type']; ?>" />
                  <input class="qw-hander-hook_key"
                         type="hidden"
                         value="<?php print $field['hook_key']; ?>" />
                  <?php print $field['title']; ?>
                </label>
                <p class="description qw-desc"><?php print $field['description']; ?></p>
                <?php
              }
            ?>
          </div>
        </div>

        <!-- add filters -->
        <div id="qw-display-add-filters" class="qw-hidden">
          <input class="add-handler-type" type="hidden" value="filter">
          <p class="description">Select filters to affect the query's results.</p>
          <div class="qw-checkboxes">
            <?php
              // loop through filters
              foreach($all_filters as $hook_key => $filter)
              {
                // for now, this is how I'll prevent certain filters on overrides
                if(in_array($query_type, $filter['allowed_query_types']))
                { ?>
                  <label class="qw-filter-checkbox">
                    <input type="checkbox"
                           value="<?php print $filter['type']; ?>" />
                  <input class="qw-hander-hook_key"
                         type="hidden"
                         value="<?php print $filter['hook_key']; ?>" />
                  <?php print $filter['title']; ?>
                  </label>
                  <p class="description qw-desc"><?php print $filter['description']; ?></p>
                  <?php
                }
              }
            ?>
          </div>
        </div>

        <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>

  <?php print theme('preview_wrapper'); ?>
</form>