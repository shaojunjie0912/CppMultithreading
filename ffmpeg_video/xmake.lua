add_requires("ffmpeg")

target("ffmpeg")
    set_kind("binary")
    set_encodings("source:utf-8")
    set_license("GPL-3.0")
    add_includedirs("include",{public=true})
    add_files("src/*.cpp")
    add_packages("ffmpeg")