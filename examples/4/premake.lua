-- 
package.name = "example4"       -- MSVS Project name
package.kind = "exe"
package.language = "c++"
package.includepaths = { "../../src" }
package.libpaths = { "../../obj" }
package.links = { "ws2_32", "mihl" }
package.files = { "example4.cpp" }
package.defines = { "WIN32", "_DEBUG", "_CONSOLE", "__WINDAUBE__" }

package.config["Debug"].target = "examples/4/example4"
package.config["Release"].target = "examples/4/example4"
