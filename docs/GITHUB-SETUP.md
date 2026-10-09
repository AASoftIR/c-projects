# Publish the repository and Pages

This package contains a ready repository, not an already created GitHub repository. Use your own GitHub account. Default configuration uses `AASoftIR/c-projects`; `scripts/publish.py` automatically uses GITHUB_REPOSITORY in Actions.

## Create it

Create an empty **public** repository named **c-projects**, with no initial README/license/gitignore. Then from this folder:

```sh
git init -b main
git add .
git commit -m "Add C Systems Forge course"
git remote add origin https://github.com/AASoftIR/c-projects.git
git push -u origin main
```

If your username differs, substitute it in the remote. Native GitHub CLI alternative:

```sh
gh auth login
gh repo create c-projects --public --source=. --remote=origin --push
```

Use one route, not both. No personal access token needs to be embedded in the project.

## Enable Pages and checklist writeback

1. Repository → Settings → Pages → Build and deployment → Source → **GitHub Actions**.
2. Settings → Actions → General → Workflow permissions: allow **Read and write permissions** if your policy otherwise prevents the explicit contents-write job. Save. The test jobs themselves have contents-read only.
3. Actions → **Course CI** → Run workflow on `main` after enabling Pages.
4. When successful, the expected site is `https://aasoftir.github.io/c-projects/`. Treat the URL as expected until the deployment succeeds; Actions reports the actual deployed URL.
5. If main is protected against direct bot commits, allow the automation identity per your policy or update README/progress locally and commit through your usual PR process. Pages can still deploy its build; the workflow reports writeback failure rather than silently claiming the README was updated.

The course workflow runs normal + sanitizer tests for both GCC and Clang, verifies the reference set separately, collects learner reports and regenerates README/progress. Only main can receive bot writeback or deployment. PRs can run tests and produce artifacts but cannot write progress or deploy Pages. It uses `pull_request`, never `pull_request_target`.

Generated commit messages include `[skip ci]`, and Pages deployment is part of the original run. No bot-triggered rebuild loop is needed. The writeback step verifies the current main SHA equals the tested SHA; it refuses to publish outdated checkboxes after a concurrent push.

## Daily workflow

```sh
git switch -c project/01-byte-lab
# Implement projects/01-byte-lab/main.c and update NOTES.md.
python3 scripts/course.py test 01
python3 scripts/course.py test 01 --sanitize
git add projects/01-byte-lab tests
git commit -m "Implement uint32 inspection with boundary tests"
git push -u origin project/01-byte-lab
```

Open a PR, inspect its checks, merge when satisfied, and let main regenerate verified progress. Do not edit the managed README block or `progress.json` by hand. Failed attempted projects stay visible with failed status; reverting to the exact starter returns them to not-started.

## Useful automation included

- CI compiler and instrumentation matrix, failure logs and reports.
- Verified README checklist with regression unticking.
- Static Pages build/deployment from current learner reports, even when an attempted project fails tests.
- Scheduled weekly reference health check (Friday at 06:00 UTC = 09:30 Tehran).
- Monthly Dependabot updates for GitHub Actions.
- Manual source bundle artifact workflow.
- Issue form for reproducible project defects and a PR template for learning evidence.

## Troubleshooting

**Bot cannot push:** inspect repository/organization write policies and branch rules. A fork may default to read-only actions; deployment also needs Pages enabled.

**No checkbox changes:** an untouched starter is intentionally skipped. Check which files were modified. Editing only NOTES.md does not make an implementation attempted.

**Report looks old:** inspect commit SHA, latest run and writeback logs. Failed tests still generate data; missing matrix artifacts are rejected so incomplete evidence cannot create a pass.

**Site fails at file://:** serve `site/` over HTTP. The dashboard loads local JSON through fetch; a file URL does not provide a normal origin.

**Sanitizer runtime unavailable:** use a normal Linux install. If ptrace blocks LeakSanitizer, disable leak detection for that local diagnostic only and keep CI unchanged.
