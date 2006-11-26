
project.name = "mihl"
-- project.libdir = "./lib"
-- project.bindir = "./bin"
project.config["Debug"].bindir  = "bin"
project.config["Release"].bindir = "bin"
project.config["Debug"].libdir  = "lib"
project.config["Release"].libdir = "lib"

dopackage("src")
dopackage("examples/1")
