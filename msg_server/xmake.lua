target("msg_server")
    set_kind("binary")
    set_encodings("source:utf-8")
    add_includedirs("include",{public=true})
    add_files("src/*.cpp")