<?php
/**
 * @file theme.inc
 * 
 * Preprocessing to provide variables to sws_fields template files.
 */

/**
 * Implementation of template_preprocess().
 */
/*
function template_preprocess_sws_fields_formatter_sws_attachment(&$variables) {
// dsm($variables);
} 
// */

/**
 * Implementation of template_preprocess().
 */
// function template_preprocess_sws_fields_video(&$variables) {
function template_preprocess_sws_fields_formatter_sws_video(&$variables) {
  $node = $variables['element']['#node'];
 
  // get variables stored in node object to pass to theme function
  $video_url = $node->field_video_url[0]['safe'];
  if ($video_url) {
    $size = $node->field_video_size[0]['safe'];
    $autoplay = check_plain($node->field_video_autoplay[0]['value']);
    $align = $node->field_video_align[0]['safe'];
  } else if ($node->node_data_field_video_url_field_video_url_value) {
    // Views doesn't actually load the whole node
    $video_url = $node->node_data_field_video_url_field_video_url_value;
    $size = _sws_fields_video_size($video_url); 
  }
  // call video filter function  provided by video_filter module 
  if ($video_url) {
    $url = check_url($video_url);
    $text = "[video: $url $size autoplay:$autoplay]";
    $video = video_filter_filter('process', 0, -1, $text);
  }

  // default to align left if no alignment is set
  $align = ($align) ? $align : 'left';

  // pass vars to sws_fields-video-field.tpl.php 
  $variables['video_url'] = $video_url;
  $variables['video'] = $video; 
  $variables['align'] = $align;
}

/**
 * Vimeo and YouTube seem to behave
 * differently in the sidebar. This 
 * attempts to find a size that will fit the 
 * 272px White House theme sidebar. 
 * 
 * TODO come up with something smarter and more flexible in the future. 
 * 
 * @param $url
 *  string, url of video
 * 
 * @return 
 *  string, size of video which will fit nicely in sidebar.
 */
function _sws_fields_video_size($url) {
  // YouTube.com
  if (strpos($url, 'youtube')) {
    return 'width:250 height:208';
  // Vimeo.com
  } else if (strpos($url, 'vimeo')) {
    return 'width:284 height:160';
  } else {
    return 'width:205 height:160'; // todo maybe this deserves its own Views handler
  }                          //  so size can be set through Views UI.
}

/**
 * Implementation of template_preprocess(). 
 */
// function template_preprocess_sws_fields_pic(&$variables) {
function template_preprocess_sws_fields_formatter_sws_pic(&$variables) {
  // This is the picture we're theming.
  $item = $variables['element']['#item'];
  // This is the node it belongs to. 
  $node = $variables['element']['#node'];

  // pics
  $pics = $node->field_pic;
  if ($pics[0]) { // make sure there are pictures to theme here

    // imagecache preset
    $preset_namespace = content_format('field_pic_size', $node->field_pic_size[0]);
    $preset_namespace = strip_tags($preset_namespace);
    $preset_namespace = trim($preset_namespace);
    // alignment
    $align = $node->field_pic_align[0]['safe'];
    $zebra = $variables['zebra'];
    $image_filepath = $item['filepath'];
    $alt = $item['data']['alt'];
    $title = $item['data']['title'];
    $attributes = ''; // TODO handle attributes
    if ($align == 'alternate') {
      $side = ($zebra == 'odd') ? 'left' : 'right';
      $output .=  "<div class='pic pic-$align-$side pic-$zebra pic-fid-". $item['fid'] ."'>";
    } else {
      $side = $align;
      $output .=  "<div class='pic pic-$align-$side pic-$zebra pic-fid-". $item['fid'] ."'>";
    }

    $pic = theme('imagecache', $preset_namespace, $image_filepath, $alt, $title, $attributes);
    $pics[] = $pic;
    $output .= $pic;

    // caption
    $description = $item['data']['description'];
    $description = check_markup($description);
    if ($description) {
      // set width for caption at width of picture.
      $image = image_get_info(imagecache_create_path($preset_namespace, $image_filepath)); 
dsm($image);
      $width = $image['width'];
      // $height = $image['height'];
      $output .= "<div class='pic-caption' style='max-width:". $width. "px'>". $description ."</div>";
    }

    $output .= '</div>';

    // pass vars to sws_fields-video-field.tpl.php 
    $variables['output'] = $output;
    $variables['pics'] = $pics;
    $variables['align'] = $align;
    $variables['size'] = $preset_namespace; 
  }
}

/**
 * Implementation of template_preprocess().
 */
// function template_preprocess_sws_fields_map(&$variables) {
function template_preprocess_sws_fields_formatter_sws_map(&$variables) {
  $node = $variables['element']['#node'];
  $type = $node->type;

  // note: embed_gmap takes care of handles filtering
  $map =  content_format('field_map', $node->field_map[0]);
  $variables['map'] = $map;
  $variables['type'] = $type;
}

