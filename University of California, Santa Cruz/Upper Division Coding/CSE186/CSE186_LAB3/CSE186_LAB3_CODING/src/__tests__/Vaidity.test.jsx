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
/*
#######################################################################
#######               DO NOT MODIFY THIS FILE               ###########
#######################################################################
*/

import {it, expect} from 'vitest';
import Entry from '../Entry';
import Picker from '../Picker';

/**
 * @param {*} component
 * @return {boolean}
 */
function isClassComponent(component) {
  let isClass = false;
  try {
    component();
  } catch (e) {
    isClass = true;
  }
  return isClass;
}

/*
 * Piekcer should be a React Class Component
 */
it('Picker is React.Component Sub Classes', async () => {
  expect(isClassComponent(Picker)).toBe(true);
});

/*
 * Entry should be a React Class Component
 */
it('Entry is React.Component Sub Classes', async () => {
  expect(isClassComponent(Entry)).toBe(true);
});
