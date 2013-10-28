<?php
  $display_types = qw_all_display_types();
?>
<!-- Preview -->
  <div id="query-preview" class="qw-query-option">
    <div id="query-preview-controls" class="query-preview-inactive">
      <?php if ($live_checkbox) { ?>
        <label>
          <input id="live-preview"
                 type="checkbox"
                 checked="checked" />
          Live Preview
        </label>
      <?php } ?>
      <select id="preview-display-type" name="preview-display-type">
        <?php
          foreach ($display_types as $key => $display_type)
          { ?>
            <option value="<?php print $key; ?>"><?php print $display_type['title']; ?></option>
            <?php
          }
        ?>
      </select>
      <div id="get-preview" class="qw-button">Preview</div>
    </div>

    <h4 id="preview-title">Preview Query</h4>
    <p><em>This preview does not include your theme's CSS stylesheet.</em></p>
    <div id="query-preview-target">
      <!-- preview -->
    </div>

    <div class="qw-clear-gone"><!-- ie hack -->&nbsp;</div>

    <div id="query-details">
      <div class="group">
        <div class="qw-setting-header">WP_Query Arguments</div>
        <div id="qw-show-arguments-target">
          <!-- args -->
        </div>
      </div>
      <div class="group">
        <div class="qw-setting-header">Resulting WP_Query Object</div>
        <div id="qw-show-wpquery-target">
          <!-- WP_Query -->
        </div>
      </div>
      <div class="group">
        <div class="qw-setting-header">Display Settings</div>
        <div id="qw-show-display-target">
          <!-- display -->
        </div>
      </div>
      <div class="group">
        <div class="qw-setting-header">Template Files</div>
        <div id="qw-show-template-files">
          <!-- Query Time -->
        </div>
      </div>
      <div class="group">
        <div class="qw-setting-header">Query Time</div>
        <div id="qw-show-query-time">
          <!-- Query Time -->
        </div>
      </div>
    </div>
  </div>