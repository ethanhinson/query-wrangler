<?php
// hook into qw_basics
add_filter('qw_basics', 'qw_basic_settings_page_template');

/*
 * Basic Settings
 */
function qw_basic_settings_page_template($basics){
  $basics['page_template'] = array(
    'title' => 'Page Template',
    'type' => 'page',
    'description' => 'Select which page template should wrap this query page.',
    'form_callback' => 'qw_basic_settings_page_template_form',
    'weight' => 0,
  );
  return $basics;
}


function qw_basic_settings_page_template_form($basic, $options)
{
  $template_file = $options['display']['page']['template-file'];
  $page_templates['Default'] = 'index.php';
  $page_templates += get_page_templates();
  ?>
    <select name="<?php print $basic['form_prefix']; ?>[template-file]">
      <?php
        foreach($page_templates as $name => $file)
        { ?>
          <option value="<?php print $file; ?>"
                  <?php if($file == $template_file) { print 'selected="selected"'; } ?> >
            <?php print $name; ?>
          </option>
          <?php
        }
      ?>
    </select>
    <p class="description"><?php print $basic['description']; ?></p>
  <?php
}