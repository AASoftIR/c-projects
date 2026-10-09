# Scope and reporting

The teaching servers bind loopback and intentionally implement a restricted HTTP subset. They are not hardened production services. If you find a memory-safety, parser, or workflow privilege issue, document the minimal reproducible input and affected version through GitHub's private vulnerability reporting if enabled; otherwise contact the maintainer through their public profile. Avoid posting real credentials or private documents. Tests run learner code in unprivileged CI jobs; publication/writeback use separate jobs restricted to main.
