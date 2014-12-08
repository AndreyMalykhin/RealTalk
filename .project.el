(ede-cpp-root-project "real_talk" 
                      :name "real_talk" 
                      :file "/Users/test/Dev/real_talk/CMakeLists.txt" 
                      :include-path '("/include")
                      :system-include-path '(
                                             "/thirdparty/gmock-1.7.0/include"
                                             "/thirdparty/gmock-1.7.0/gtest/include"
                                             )
)

(setq company-c-headers-path-user (quote ("/Users/test/Dev/real_talk/include")))
(setq company-clang-arguments (quote ("-I/Users/test/Dev/real_talk/include")))

(setq compile-command "cd /Users/test/Dev/real_talk/build && cmake -DCMAKE_BUILD_TYPE=Debug .. && make -j 8 VERBOSE=1 && ./real_talk_tests")

(require 'google-c-style)
(defun my:enable-google-c-style()
  (google-set-c-style)
  (google-make-newline-indent)
)
(add-hook 'c-mode-common-hook 'my:enable-google-c-style)

(require 'flymake-google-cpplint)
(add-hook 'c-mode-hook 'flymake-google-cpplint-load)
(add-hook 'c++-mode-hook 'flymake-google-cpplint-load)
