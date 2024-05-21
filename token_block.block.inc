<?php

class TokenBlock extends Block {

  /**
   * {@inheritdoc}
   */
  function __construct($plugin_name, array $data) {
    parent::__construct($plugin_name, $data);

    $this->settings += array(
      'content' => '',
      'format' => NULL,
    );
  }

  /**
   * {@inheritdoc}
   */
  function getTitle() {
    $entity_type = $this->childDelta;
    if (!empty($this->settings['title'])) {
      if (empty($this->contexts[$entity_type]->data)) {
        $title = $this->settings['title'];
      }
      else {
        $data = array($entity_type => $this->contexts[$entity_type]->data);
        // Sanitization is done later, so that we can remove markup first.
        $replaced = token_replace($this->settings['title'], $data, array('sanitize' => FALSE));
        // Remove any markup, as block titles are not supposed to use any.
        $title = strip_tags($replaced);
      }
      // Perform check_plain, since it's not done by token_replace.
      return check_plain($title);
    }
    else {
      return t('Token block');
    }
  }

  /**
   * {@inheritdoc}
   */
  function form(&$form, &$form_state) {
     parent::form($form, $form_state);
    $variables = array(
      'token_types' => array($this->childDelta),
    );

    $form['content'] = array(
      '#type' => 'text_format',
      '#title' => t('Block content'),
      '#default_value' => $this->settings['content'],
      '#format' => $this->settings['format'],
      '#editor_uploads' => TRUE,
      '#rows' => 5,
      '#description' => theme('token_tree_link', $variables),
    );
  }

  /**
   * {@inheritdoc}
   */
  function formSubmit($form, &$form_state) {
    parent::formSubmit($form, $form_state);
    $this->settings['content'] = $form_state['values']['content']['value'];
    $this->settings['format'] = $form_state['values']['content']['format'];
  }

  /**
   * Returns the rendered content of this block.
   */
  function getContent() {
    if (!empty($this->settings['content'])) {
      $entity_type = $this->childDelta;
      $entity = $this->contexts[$entity_type]->data;
      $data = array($entity_type => $entity);
      return check_markup(token_replace($this->settings['content'], $data), $this->settings['format'], '', TRUE);
    }
  }

  /**
   * Creates an array of "child blocks" for display in the "Add Block" dialog.
   *
   * This allows a single entry in hook_block_info() to provide multiple
   * block instances displaying different fields.
   *
   * @see token_block_get_block_list()
   */
  function getChildren() {
    return token_block_get_block_list();
  }
}
