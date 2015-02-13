# nodejs-fdf
Create FDF files with nodejs

How to install this module?
==================================

First of all you have to install node-gyp
```
npm -g install node-gyp rebuild 
```

After that you can install the fdf module
```
git clone https://github.com/agrippa1994/nodejs-fdf.git
npm install nodejs-fdf/
rm -rf nodejs-fdf/
```

How to use this module inside your script?
===========================================
```
var fdf = require("fdf");

try{
	console.log(fdf.createFDF("test.fdf", {name_verkaeufer: "ValueÃ¤Ã¶Ã¼ÃœÃ–Ã„ÃŸ"}));
} catch(e) {
	console.log("Exception: " + e);
}
```
