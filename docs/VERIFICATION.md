# Verification record for the source package

Verified locally on the supplied Linux runtime with GCC 13.3.0 and Python 3:

- All 20 reference baselines passed strict normal builds and their contract tests: **581 assertions**.
- All 20 reference baselines passed AddressSanitizer/UndefinedBehaviorSanitizer builds: **582 assertions** (the shared-library project uses a linked C harness in this lane).
- Leak detection was **disabled locally** because LeakSanitizer could not inspect processes in this runtime. This is not evidence of a leak-free result. The included CI retains `detect_leaks=1` for normally exiting programs. Long-running servers are terminated by the tests, so exit-time leak accounting and graceful shutdown remain extensions even in CI.
- Five infrastructure regression tests passed: matrix completeness, failure regression, reference exclusion, invalid/mixed evidence rejection, and an actual copied-repository starter → implementation pass → regression sequence.
- JavaScript syntax and workflow YAML parsing checks passed.

**Not verified here:** Clang execution, browser visual rendering or browser interaction, actual GitHub workflow execution, remote bot writeback, and live Pages deployment. The local browser preview was unavailable. The code and publishing instructions are included, but no live URL is claimed.

The shipped learner checklist starts at **0/20, not started**. Reference verification is separate from your progress. CI is designed to collect GCC and Clang normal/sanitizer learner evidence before ticking a checkbox; it must run in your repository to establish those results.

Tests establish the documented baseline contracts, not complete production robustness or mastery of C. Larger extension tracks need new tests, new failure contracts and their own review.
