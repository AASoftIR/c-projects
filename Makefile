PYTHON ?= python3
PROJECT ?= 01
.PHONY: list test sanitize reference report site clean
list:
	$(PYTHON) scripts/course.py list
test:
	$(PYTHON) scripts/course.py test $(PROJECT)
sanitize:
	$(PYTHON) scripts/course.py test $(PROJECT) --sanitize
reference:
	$(PYTHON) scripts/course.py test all --mode reference
report:
	$(PYTHON) scripts/course.py report --output progress.local.json
site:
	$(PYTHON) scripts/publish.py --reports progress.local.json
	$(PYTHON) -m http.server 8000 --directory site
clean:
	rm -rf build
