-- 
package.name = "example2"       -- MSVS Project name
package.kind = "exe"
package.language = "c++"
package.includepaths = { "../../src" }
package.libpaths = { "../../obj" }
package.links = { "ws2_32", "mihl" }
package.files = { "example2.cpp" }
package.defines = { "WIN32", "_DEBUG", "_CONSOLE" }

package.config["Debug"].target = "examples/2/example2"
package.config["Release"].target = "examples/2/example2"
