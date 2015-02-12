<?php
/**
 * @file
 * The admin file containing the menu callback function.
 */

/**
 * Menu callback for the String Overrides module to display administration
 */

function parsely_admin_settings($form, &$form_state) {
  // Required settings
  $form['parsely_apikey'] = array(
    '#type' => 'textfield',
    '#title' => t('Parse.ly Site ID'),
    '#description' => t('You can find your Site ID on your your <a target="_blank" href="@parsely_settings">API settings page</a>.',
              array('@parsely_settings' => 'http://dash.parsely.com/to/settings/api?highlight=apikey')),
    '#default_value' => variable_get('parsely_apikey'),
  );

  if (module_exists('taxonomy')) {
    $form['parsely_sections_wrapper'] = array(
      '#type' => 'fieldset',
      '#title' => t('Section Settings'),
      '#collapsible' => FALSE,
      '#collapsed' => FALSE,
    );
    $form['parsely_sections_wrapper']['parsely_section_vocabulary'] = array(
      '#type' => 'radios',
      '#title' => t('Section Vocabulary'),
      '#options' => _parsely_vocab_array_format(taxonomy_get_vocabularies()),
      '#description' => t('Select the taxonomy to use for Parse.ly sections. By default, the first matching term in this vocabulary for a given page is used to populate the Parse.ly section variable.'),
      '#default_value' => variable_get('parsely_section_vocabulary', ''),
    );
    $form['parsely_sections_wrapper']['parsely_section_term_criteria'] = array(
      '#title' => t('Section term selection criteria'),
      '#type' => 'radios',
      '#options' => array(
        'first' => t('First term'),
        'last' => t('Last term'),
        'highest' => t('Highest-level term'),
        'ancestor' => t('Highest-level ancestor'),
      ),
      // Provide a long-winded explanation of how this impacts term selection.
      // Note that there's currently no way to say "find the last/newest term,
      // and then select its highest-level ancestor" or "use the term that
      // appears first in the set of terms for a given term/entity reference
      // field," etc.
      '#description' => t('If a given node/page may be associated with multiple terms from the vocabulary you selected above, this setting can help determine which term to use. "First term" and "Last term" will choose a term based on the time at which they were associated with the given node (rather than the order in which they appear within a term reference field). "Highest term" will choose the term that lives closest to the root of the vocabulary (defaulting to the "first"/oldest of such terms if more than one exist). For hierarchical vocabularies, you can also opt to use the "Highest-level ancestor" of a given term, which means that if a given node is tagged with a lower-lever term (e.g. corresponding to a subsection of your site), we will report that term\'s parent (or grandparent, etc.) term as the section. Wherever a choice must be made between terms at the same depth, the first term will be chosen.'),
      '#default_value' => variable_get('parsely_section_term_criteria', 'first'),
    );

    $form['parsely_tag_vocabularies'] = array(
      '#type' => 'checkboxes',
      '#title' => t('Tag Vocabularies'),
      '#options' => _parsely_vocab_array_format(taxonomy_get_vocabularies()),
      '#description' => t('Select which taxonomy vocabularies will be tracked as tags.'),
      '#default_value' => variable_get('parsely_tag_vocabularies', array()),
    );
  }
  // TODO: Should display warning if taxonomy module isn't available

  // Optional settings
  $form['parsely_optional_settings'] = array(
    '#type' => 'fieldset',
    '#title' => t('Optional Settings'),
    '#collapsible' => TRUE,
    '#collapsed' => TRUE,
  );
  $form['parsely_optional_settings']['parsely_content_id_prefix'] = array(
    '#type' => 'textfield',
    '#title' => t('Content ID Prefix'),
    '#description' => t('If you use more than one content management system (e.g. Drupal and WordPress), you may end up with duplicate content IDs. Adding a Content ID Prefix will ensure the content IDs from Drupal will not conflict with other content management systems.'),
    '#default_value' => variable_get('parsely_content_id_prefix', ''),
  );
  $form['parsely_optional_settings']['parsely_track_authenticated_users'] = array(
    '#type' => 'radios',
    '#title' => t('Track Authenticated Users'),
    '#description' => t('By default, the Parse.ly module will track the activity of users that are logged into this site. You can change this setting to only track the activity of anonymous visitors. Note: You will no longer see the Parse.ly tracking code on your site if you browse while logged in.'),
    '#options' => array(0 => t('No'), 1 => t('Yes')),
    '#default_value' => variable_get('parsely_track_authenticated_users', 1),
  );


  return system_settings_form($form);
}


function _parsely_vocab_array_format($vocabularies = NULL) {
  $vocab_array = array();
  foreach ($vocabularies as $vocab) {
    $vocab_array[$vocab->vid] = $vocab->name;
  }

  return $vocab_array;
}
