-- 
package.name = "mihl"        -- MSVS Project name
package.kind = "dll"
package.language = "c++"
package.includepaths = { "../" }
package.config["Debug"].objdir = "obj/debug"
package.config["Release"].objdir = "obj/debug"
-- package.libpaths = { "" }
package.links = { "ws2_32" }
package.defines = { "__WINDAUBE__" }
package.files = {
    "mihl.h", "glovars.h", "mihl.cpp",
    "tcp_utils.h", "tcp_utils.cpp"
}

package.config["Debug"].target = "lib/mihl"
package.config["Release"].target = "lib/mihl"
