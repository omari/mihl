-- 
package.name = "example3"       -- MSVS Project name
package.kind = "exe"
package.language = "c++"
package.includepaths = { "../../src" }
package.libpaths = { "../../obj" }
package.links = { "ws2_32", "mihl" }
package.files = { "example3.cpp" }
package.defines = { "WIN32", "_DEBUG", "_CONSOLE", "__WINDAUBE__" }

package.config["Debug"].target = "examples/3/example3"
package.config["Release"].target = "examples/3/example3"
