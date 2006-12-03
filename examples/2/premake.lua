-- 
package.name = "example2"       -- MSVS Project name
package.kind = "exe"
package.language = "c++"
package.includepaths = { "../../src" }
package.libpaths = { "../../obj" }
package.links = { "ws2_32", "mihl" }
package.files = { "example1.cpp" }
package.defines = { "WIN32", "_DEBUG", "_CONSOLE", "__WINDAUBE__" }

package.config["Debug"].target = "examples/2/example2"
package.config["Release"].target = "examples/2/example2"
