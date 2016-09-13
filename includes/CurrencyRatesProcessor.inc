<?php

/**
 * @file
 * CurrencyRatesProcessor class.
 */

/**
 * Feeds processor plugin. Change currency rates from feed items.
 */
class CurrencyRatesProcessor extends FeedsProcessor {

  protected $conversion_settings;
  
  protected function getConversionSettings() {
    if (empty($this->conversion_settings)) {
      $this->conversion_settings = variable_get('commerce_multicurrency_conversion_settings', array());
    }
    return $this->conversion_settings;
  }

  protected function setConversionSettings($settings) {
    variable_set('commerce_multicurrency_conversion_settings', $settings);
    $this->conversion_settings = $settings;
  }

  /**
   * Define entity type.
   */
  public function entityType() {
    return 'currency_rate';
  }

  /**
   * Implements parent::entityInfo().
   */
  protected function entityInfo() {
    $info = parent::entityInfo();
    $info['label plural'] = t('Currency Rates');
    $info['bundle name'] = t('Without bundle');
    return $info;
  }

  /**
   * Creates a new rate in memory and returns it.
   */
  protected function newEntity(FeedsSource $source) {
    $rate = new stdClass();
    $rate->currency_to_save = $source->config['FeedsExcelParser']['currency_to_save'];
    return $rate;
  }

  /**
   * Validates a rate.
   */
  protected function entityValidate($rate) {
    if (!is_numeric($rate->currency_rate)) {
      throw new FeedsValidationException(t('Currency Rate is not numeric'));
    }
  }

  /**
   * Saves a rate.
   */
  protected function entitySave($rate) {
    if ($rate->currency_rate > 0) {
      $settings = $this->getConversionSettings();
        $settings[$rate->currency_to_save]['rates'][$rate->currency_code]['rate'] = (float) sprintf('%.4F', $rate->currency_rate);
        $this->setConversionSettings($settings);
    }
  }

  /**
   * Deletes a series of rates.
   */
  protected function entityDeleteMultiple($rates) {
    // Do nothing.
  }

  /**
   * Override parent::configDefaults().
   */
  public function configDefaults() {
    return parent::configDefaults();
  }

  /**
   * Overrides parent::setTargetElement().
   */
  public function setTargetElement(FeedsSource $source, $target_term, $target_element, $value, array $mapping = array()) {
    parent::setTargetElement($source, $target_term, $target_element, $value);
  }

  /**
   * Return available mapping targets.
   */
  public function getMappingTargets() {
    $targets = parent::getMappingTargets();
    $targets += array(
      'currency_code' => array(
        'name' => t('Currency Code'),
        'description' => t('Code of currency in iso2 format'),
        'optional_unique' => FALSE,
      ),
      'currency_rate' => array(
        'name' => t('Rate for current currency'),
        'description' => t('Decimal number'),
        'optional_unique' => FALSE,
      ),
    );

    $this->getHookTargets($targets);

    return $targets;
  }

  /**
   * Get id of an existing feed item rate if available.
   */
  protected function existingEntityId(FeedsSource $source, FeedsParserResult $result) {
    // Return always zero.
    return 0;
  }

  /**
   * Overrides FeedsProcessor::dependencies().
   */
  public function dependencies() {
    $dependencies = parent::dependencies();
    return $dependencies;
  }

}
