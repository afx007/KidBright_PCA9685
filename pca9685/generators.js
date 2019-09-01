/*
Blockly.JavaScript["setFreq"] = function(block) {
  var value_Freq = Blockly.JavaScript.valueToCode(block, 'Freq', Blockly.JavaScript.ORDER_ATOMIC);
  var code = 'DEV_I2C1.PCA9685(0,0x40).setPWMFreq('+value_Freq+');\n';
  return code;
};
*/
Blockly.JavaScript['SetPwm'] = function(block) {
    var dropdown_pin = block.getFieldValue('PIN');
    var number_spd = block.getFieldValue('SPD');
    var code = 'DEV_I2C1.PCA9685(0,0x40).setPWM('+dropdown_pin+','+number_spd+');\n';
    return code;
  };