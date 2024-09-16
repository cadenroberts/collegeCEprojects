
const pixelmatch = require('pixelmatch');
const fs = require('fs');
const PNG = require('pngjs').PNG;

async function compare(page, name) {
  await page.screenshot({path: `${process.cwd()}/${name}.png`});
  return new Promise((resolve, reject) => {
    const received = fs.createReadStream(`${process.cwd()}/${name}.png`).pipe(new PNG()).on('parsed', doneReading);
    const expected = fs.createReadStream(`${process.cwd()}/shots/${name}.png`).pipe(new PNG()).on('parsed', doneReading);

    let filesRead = 0;
    function doneReading() {
      if (++filesRead < 2) return;

      const diff = new PNG({width: expected.width, height: expected.height});
      const numDiffPixels = pixelmatch(
          expected.data, received.data, diff.data, expected.width, expected.height,
          {threshold: 0.1});

      expect(numDiffPixels).toEqual(0);
      resolve();
    }
  });
}

module.exports = { compare };
