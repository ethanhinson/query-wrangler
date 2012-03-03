<?php
// add default fields to the hook filter
add_filter('qw_fields', 'qw_field_image_attachment');

/*
 * Add field to qw_fields
 */
function qw_field_image_attachment($fields){
  $fields['image_attachment'] = array(
    'title' => 'Image Attachment',
    'description' => 'Image files that are attached to a post.',
    'output_callback' => 'qw_theme_image',
    'output_arguments' => true,
    'form_callback' => 'qw_field_image_attachment_form',
  );
  return $fields;
}

/*
 * Image attachment settings
 */
function qw_field_image_attachment_form($field)
{
  $image_sizes = get_intermediate_image_sizes();
  ?>
    <!-- image display -->
    <label class="qw-label">Number of items to show:</label>
    <input class="qw-text-short qw-title-ignore"
           type="text"
           name='<?php print $field['form_prefix']; ?>[image_display_count]'
           value="<?php print ($field['values']['image_display_count']) ? $field['values']['image_display_count']: 0;?>" />

    <p>
      <label class="qw-label">Image Display Style:</label>
      <select name='<?php print $field['form_prefix']; ?>[image_display_style]'>
        <?php
          foreach($image_sizes as $key => $size)
          {
            $size_selected = ($field['values']['image_display_style'] == $size) ? 'selected="selected"' : '';
            ?>
              <option value="<?php print $size; ?>" <?php print $size_selected; ?>><?php print $size; ?></option>
            <?php
          }
        ?>
      </select>
    </p>
  <?php
}

/*
 * Turn a list of images into html
 *
 * @param $post_id
 * @param $image_type
 * @param $count;
 */
function qw_theme_image($post, $field)
{
  $style = $field['image_display_style'];
  $count = $field['image_display_count'];

  $images = qw_get_post_images($post->ID);
  if(is_array($images)){
    $output = '';
    $i = 0;
    foreach($images as $image){
      if($count == 0 || ($i < $count)){
        $output.= wp_get_attachment_image($image->ID, $style);
      }
      $i++;
    }
    return $output;
  }
}
/*
 * Get all images attached to a single post
 *
 * @param int $post_id The Wordpress ID for the post or page to get images from
 *
 * @return sorted array of images
 */
function qw_get_post_images($post_id)
{
		$child_args = array(
    "post_type" => "attachment",
    "post_mime_type" => "image",
    "post_parent" => $post_id
  );
		// Get images for this post
  $images = &get_children($child_args);

		// If images exist for this page
		if(is_array($images))
		{
				// sort this so menu order matters
				$sorted = array();
    $unsorted = array();
				foreach ($images as $image)
				{
      if($image->menu_order !== 0){
        $sorted[$image->menu_order] = $image;
      }
      else {
        $unsorted[] = $image;
      }
				}
    // sort menu order
    ksort($sorted);
    // reset array
    $sorted = array_values($sorted);
    // add unsorted
    $sorted = array_merge($sorted, $unsorted);

				return $sorted;
		}
}