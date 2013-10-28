<!-- <?php print $handler['title']; ?> -->
  <div id="qw-query-<?php print $handler['hook_key']; ?>" class="qw-query-admin-options">
    <div class="qw-query-add-titles">
      <span class="qw-rearrange-title" title="qw-sort-<?php print $handler['hook_key']; ?>">
        Rearrange
      </span>
      <span class="qw-query-title" title="qw-display-add-<?php print $handler['hook_key']; ?>">
        Add
      </span>
    </div>
    <h4><?php print $handler['title']; ?></h4>
    <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>

    <div id="qw-query-<?php print $handler['hook_key']; ?>-list">
      <?php
        if(is_array($handler['items']))
        {
          // loop through and display
          foreach($handler['items'] as $item)
          { ?>
              <div class="qw-query-title" title="qw-<?php print $handler['hook_key']; ?>-<?php print $item['name']; ?>">
                <span class="qw-setting-title"><?php print $item['title'];  ?></span>
                :
                <span class="qw-setting-value"><?php print $item['name']; ?></span>
              </div>
            <?php
          }
        }
      ?>
    </div>
  </div>
  <!-- /<?php print $handler['hook_key']; ?>s -->