# Contributing

Implement learner code in projects/. Keep references intact unless fixing a verified defect. Extend tests when you widen a contract. Do not treat reference results as learner completion. Run the touched project normally and with sanitizers, and update NOTES.md with a design decision. PRs must describe inputs/outputs, ownership, test evidence and remaining limits. All normal learner .c/.h files must live under that project folder so attempt detection remains accurate; promoted shared code should be copied into later project folders with its ownership documented.
