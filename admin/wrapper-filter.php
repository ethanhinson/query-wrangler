<?php
/*
 * $filter: array of default filter data, and specific filter['values']
 */
?>
<!-- <?php print $filter['name']; ?> -->
<div id="qw-filter-<?php print $filter['name']; ?>" class="qw-filter qw-sortable-item qw-item-form">
  <span class="qw-setting-header">
    <?php print $filter['title']; ?>
  </span>
  <div class="group">
    <input class='qw-filter-type'
           type='hidden'
           name='<?php print $filter['form_prefix']; ?>[type]'
           value='<?php print $filter['type']; ?>' />
    <input class='qw-filter-hook_key'
           type='hidden'
           name='<?php print $filter['form_prefix']; ?>[hook_key]'
           value='<?php print $filter['hook_key']; ?>' />
    <input class='qw-filter-name'
           type='hidden'
           name='<?php print $filter['form_prefix']; ?>[name]'
           value='<?php print $filter['name']; ?>' />

    <div class="qw-remove button">
      Remove
    </div>
    <input class='qw-weight'
           name='<?php print $filter['form_prefix']; ?>[weight]'
           type='text' size='2' tabindex="9999"
           value='<?php print $filter['weight']; ?>' />

    <p class="description"><?php print $filter['description']; ?></p>

    <?php
      if (isset($filter['form']))
      { ?>
        <div class="qw-filter-form">
          <?php print $filter['form']; ?>
        </div>
        <?php
      }

      // exposed form and settings
      if(isset($filter['exposed_form']))
      {
        $is_exposed = (isset($filter['values']['is_exposed'])) ? 'checked="checked"': '';
        $limit_values = (isset($filter['values']['exposed_limit_values'])) ? 'checked="checked"': '';
        $default_values = (isset($filter['values']['exposed_default_values'])) ? 'checked="checked"': '';
        ?>
        <div class="qw-exposed-form">
          <div class="qw-setting">
            <label class="qw-label">Expose Filter:</label>
            <p>
              <input type="checkbox"
                      name='<?php print $filter['form_prefix']; ?>[is_exposed]'
                      <?php print $is_exposed; ?> />
            </p>
            <p class="description">
              Exposing a filter allows a site guest to alter the query results with a form.
              <br />If you expose this filter, the values above will act as the default values of the filter.</p>
          </div>
          <div>
            <label class="qw-label">Limit Values:</label>
            <p>
              <input type="checkbox"
                      name='<?php print $filter['form_prefix']; ?>[exposed_limit_values]'
                      <?php print $limit_values; ?> />
            </p>
            <p class="description">If checked, only the values above will be available to the exposed filter.</p>
          </div>
          <div>
            <label class="qw-label">Default Values:</label>
            <p>
              <input type="checkbox"
                      name='<?php print $filter['form_prefix']; ?>[exposed_default_values]'
                      <?php print $default_values; ?> />
            </p>
            <p class="description">If checked, the values above will be the default values of the exposed filter.</p>
          </div>
          <div>
            <label class="qw-label">Exposed Label:</label>
            <input type="text"
                   name='<?php print $filter['form_prefix']; ?>[exposed_label]'
                   value="<?php print $filter['values']['exposed_label']; ?>" />
            <p class="description">Label for the exposed form item.</p>
          </div>
          <div>
            <label class="qw-label">Exposed Description:</label>
            <input class="qw-text-long"
                   type="text"
                   name='<?php print $filter['form_prefix']; ?>[exposed_desc]'
                   value="<?php print $filter['values']['exposed_desc']; ?>" />
            <p class="description">Useful for providing help text to a user.</p>
          </div>
          <div>
            <label class="qw-label">Exposed Key:</label>
            <input type="text"
                   name='<?php print $filter['form_prefix']; ?>[exposed_key]'
                   value="<?php print $filter['values']['exposed_key']; ?>" />
            <p class="description">URL ($_GET) key for the filter.  Useful for multiple forms on a single page.</p>
          </div>
          <?php if ($filter['exposed_settings_form']){ ?>
            <div class="qw-exposed-settings-form">
              <?php print $filter['exposed_settings_form']; ?>
            </div>
          <?php } ?>
        </div>
        <?php
      }
    ?>
  </div>
</div>
