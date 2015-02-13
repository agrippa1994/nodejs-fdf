var fdf = require("fdf");

try{
	console.log(fdf.createFDF("test.fdf", {name_verkaeufer: "ValueäöüÜÖÄß"}));
} catch(e) {
	console.log("Exception: " + e);
}
