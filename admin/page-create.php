<div>
  <p>
    Choose the name and the display types of your query.
  </p>
</div>

<div id="qw-create">
  <form action='admin.php?page=query-wrangler&action=create&noheader=true' method='post'>
    <div class="qw-setting">
      <label class="qw-label">Query Name:</label>
      <input class="qw-create-input" type="text" name="qw-name" value="" />
      <p class="description">Query name is a way for you, the admin, to identify the query easily.</p>
    </div>

    <div class="qw-setting">
      <h4>Display Types</h4>
      <p class="description">How should the query bew presented to users?</p>
      <?php
        $display_types = qw_all_display_types();
        foreach ($display_types as $key => $display_type)
        { ?>
          <div class="clear-left">
            <label class="qw-label"><?php print $display_type['title']; ?></label>
            <input type="checkbox"
                   name="qw-display[types][<?php print $key; ?>]"
                   value="<?php print $key; ?>" />

            <p class="description clear-left"><?php print $display_type['description']; ?></p>
          </div>
          <?php
        }
      ?>
    </div>

    <div class="qw-setting">
      <label class="qw-label">Data:</label>
      <select name="qw-data-wizard" id="qw-data-wizard">
      <?php
        foreach($wizards as $name => $wizard)
        { ?>
          <option value="<?php print $name; ?>"><?php print $wizard['title']; ?></option>
          <?php
        }
      ?>
      </select>
      <p class="description">Choose the starting data for your query.</p>
      <div id="wizard-descriptions">
        <?php
          foreach ($wizards as $name => $wizard)
          { ?>
            <p id="wizard-<?php print $name; ?>" class="description"><?php print $wizard['description']; ?></p>
            <?php
          }
        ?>
      </div>
    </div>

    <div class="qw-create">
      <input type="submit" value="Create" class="button-primary" />
    </div>
  </form>
</div>

<div id="qw-create-description">
  <?php
    foreach ($display_types as $display_type)
    { ?>
      <div>
        <h3><?php print $display_type['title']; ?></h3>
        <p>
          <?php print $display_type['help']; ?>
        </p>
      </div>    
      <?php
    }
  ?>
</div>