/*
#######################################################################
#
# Copyright (C) 2020-2024 David C. Harrison. All right reserved.
#
# You may not use, distribute, publish, or modify this code without
# the express written permission of the copyright holder.
#
#######################################################################
*/

/**
 * CSE186 Assignment 2 - Basic
 */

// eslint-disable-next-line
class Templater {
  /**
   * Replace the contents of {{ }} tagged table header and data
   * elements in document with values found in the supplied JSON
   * @param {object} document
   * @param {string} json with propeties matching tags in document
   */
  byTag(document, json) {
    document.querySelectorAll('th, td').forEach(element => {
      element.innerHTML = element.innerHTML.replace(/{{\s*(\w+)\s*}}/g, (match, p1) => {
        return JSON.parse(json)[p1] !== undefined ? JSON.parse(json)[p1] : match;
      });
    });
  }
  /**
   * Replace the contents of table header and data elements in
   * in document with id'd content found in the supplied JSON
   * @param {object} document
   * @param {string} json with propeties matching element ids in document
   */
  byId(document, json) {
    Object.keys(JSON.parse(json)).forEach(id => {
      if (document.getElementById(id)) document.getElementById(id).innerHTML = JSON.parse(json)[id];
    });
  }
}
