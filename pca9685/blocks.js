/*
Blockly.Blocks["setFreq"] = {
  init: function() {
    this.appendValueInput("Freq")
      .setCheck("Number")
      .appendField(Blockly.Msg.Freq);
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(300);
    this.setTooltip("");
    this.setHelpUrl("");
  }
};
*/
Blockly.Blocks["SetPwm"] = {
  init: function() {
    this.appendDummyInput()
    .appendField("PCA9685")
     // .appendField(Blockly.Msg.PWM)
      .appendField(
        new Blockly.FieldDropdown([
          ["PIN0", "0"],
          ["PIN1", "1"],
          ["PIN2", "2"],
          ["PIN3", "3"],
          ["PIN4", "4"],
          ["PIN5", "5"],
          ["PIN6", "6"],
          ["PIN7", "7"],
          ["PIN8", "8"],
          ["PIN9", "9"],
          ["PIN10", "10"],
          ["PIN11", "11"],
          ["PIN12", "12"],
          ["PIN13", "13"],
          ["PIN14", "14"],
          ["PIN15", "15"]
        ]),
        "PIN"
      );
    this.appendDummyInput()
      .appendField(Blockly.Msg.SPD)
      .appendField(new Blockly.FieldNumber(0, 0, 100, 1), "SPD");
    this.setInputsInline(true);
    this.setPreviousStatement(true, null);
    this.setNextStatement(true, null);
    this.setColour(0);
    this.setTooltip(Blockly.Msg.SPDINFO);
    this.setHelpUrl(Blockly.Msg.MyURL);
  }
};