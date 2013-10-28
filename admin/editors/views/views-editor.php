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
  <div id="message" class="updated qw-changes">
    <p><strong>*</strong> Changes have been made that need to be saved.</p>
  </div>

  <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>

  <div id="qw-query-admin-options-wrap">

<!-- left column -->
    <div class="qw-query-admin-column">
      <div id="qw-query-args" class="qw-query-admin-options">
        <h4>Basic settings</h4>
        <?php
          foreach($basics as $basic)
          {
            // make sure item has form
            if (isset($basic['form']))
            { ?>
                <div class="qw-query-title" title="qw-<?php print $basic['hook_key']; ?>">
                  <?php print $basic['title']; ?>
                  :
                  <span class="qw-setting-value">
                    <?php
                      if (isset($options[$basic['type']][$basic['hook_key']])){
                        print $options[$basic['type']][$basic['hook_key']];
                      }
                    ?>
                  </span>
                </div>
              <?php
            }
          }
        ?>
      </div> <!-- /qw-query-args -->
      <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>
    </div>
    <!-- /column -->

<!-- middle column -->
    <div class="qw-query-admin-column">
      <?php
        // add contextual_filters and fields
        $handler_types = array('field');

        foreach ($handler_types as $type){
          $handler = $handlers[$type];
          print theme('editor_views_add_handler', array('handler' => $handler));
        }
      ?>
    </div>
    <!-- /middle column -->

<!-- right column -->
    <div class="qw-query-admin-column">
      <?php
        // add sorts and add filters
        $handler_types = array('sort', 'filter');

        foreach ($handler_types as $type){
          $handler = $handlers[$type];
          print theme('editor_views_add_handler', array('handler' => $handler));
        }
      ?>
    </div>
    <!-- /right column -->
    <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>
  </div>

<!-- ------- FORMS --------- -->
      <div id="qw-options-forms">
<!-- Basic Settings -->
        <?php
          foreach($basics as $basic)
          {
            if(isset($basic['form']))
            { ?>
              <div id="qw-<?php print $basic['hook_key']; ?>" class="qw-item-form">
                <?php
                  print $basic['form'];
                ?>
              </div>
              <?php
            }
          }
        ?>

<!-- Edit Existing handlers Forms -->
        <?php
          // loop through existing items per handler
          $handler_types = array('sort','filter','field', 'contextual_filter');
          foreach($handler_types as $type)
          {
            if (isset($handlers[$type]))
            {
              $handler = $handlers[$type];
              ?>
              <!-- edit <?php print $type; ?>s -->
              <div id="existing-<?php print $type; ?>s">
                <?php
                  if (is_array($handler['items'])){
                    // tokens for fields
                    $tokens = array();
  
                    foreach($handler['items'] as $name => $item)
                    {
                      $args = array(
                        $type => $item,
                      );
  
                      // if this is a field, add it's token and pass as template variable
                      if ($handler['hook_key'] == 'field') {
                        $tokens[] = '{{'.$item['name'].'}}';
                        $args['tokens'] = $tokens;
                      }
                      print theme('query_'.$type, $args);
                    }
                  }
                ?>
              </div>
              <?php
            }
          }
        ?>

<!-- Add Handlers Forms -->
        <?php
          $handler_types = array('sort','filter','field', 'contextual_filter');

          foreach ($handler_types as $type)
          {
            if (isset($handlers[$type]))
            {
              $handler = $handlers[$type];
              ?>
              <!-- add sorts -->
              <div id="qw-display-add-<?php print $type; ?>" class="qw-hidden">
                <input class="add-handler-type" type="hidden" value="<?php print $type; ?>">
                <p class="description"><?php print $handler['description']; ?></p>
                <div class="qw-checkboxes">
                  <?php
                    // loop through sorts
                    foreach($handler['all_items'] as $item_key => $item)
                    {
                      ?>
                      <label>
                        <input type="checkbox"
                               value="<?php print $item['type']; ?>" />
                        <input class="qw-hander-hook_key"
                               type="hidden"
                               value="<?php print $item['hook_key']; ?>" />
                        <?php print $item['title']; ?>
                      </label>
                      <p class="description qw-desc"><?php print $item['description']; ?></p>
                      <?php
                    }
                  ?>
                </div>
              </div>
              <?php
            }
          }
        ?>
    </div><!-- options forms -->

  <?php print theme('preview_wrapper', array('live_checkbox' => true)); ?>
</form>