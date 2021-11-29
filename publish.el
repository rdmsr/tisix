(package-initialize)
(add-to-list
 'package-archives
 '("melpa" . "http://melpa.org/packages/"))


(require 'weblorg)
(require 'ox-html)

(use-package htmlize
  :ensure t
  :config
  (setq org-html-htmlize-output-type 'css))

(use-package rainbow-delimiters :config :ensure t)

(add-hook 'prog-mode-hook #'rainbow-delimiters-mode)

;; route for rendering each post

(weblorg-route
 :name "posts"
 :input-pattern "posts/*.org"
 
;; :input-aggregate #'weblorg-input-aggregate-all-desc
 :template "page.html"
 :output "posts/{{ slug }}.html"
  :site (weblorg-site :base-url "")
 :url "posts/{{ slug }}.html")

;; route for rendering the index page of the blog
(weblorg-route
 :name "blog"
 :input-pattern "posts/*.org"
 :input-aggregate #'weblorg-input-aggregate-all-desc
 :template "blog.html"
 :output "index.html"
  :site (weblorg-site :base-url "") 
  :url "/")

(weblorg-route
 :name "atom"
 :input-pattern "posts/*.org"
 :input-aggregate #'weblorg-input-aggregate-all-desc
 :template "atom.html"
 :output "feed.xml"
  :site (weblorg-site :base-url "")
  :url "/")


;; grab the static stuff
(weblorg-copy-static
 :output "style/{{ file }}"
 :url "style/{{ file }}")

;; fire the engine and export all the files declared in the routes above
(weblorg-export)

