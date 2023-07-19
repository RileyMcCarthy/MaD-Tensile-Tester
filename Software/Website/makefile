VENV = venv
PYTHON = $(VENV)/bin/python3
PIP = $(VENV)/bin/pip
ACTIVATE = $(VENV)/bin/activate
STATIC = app/static

.PHONY: run clean

run:
	$(PYTHON) MaD.py
	#. $(ACTIVATE); export FLASK_APP=MaD.py; flask run --host=0.0.0.0

install: static venv

static: 
	cd $(STATIC); npm install

venvc: clean $(VENV)/bin/activate

venv: $(VENV)/bin/activate
	$(PIP) install -r requirements.txt

$(VENV)/bin/activate: requirements.txt
	python3 -m venv $(VENV)
	$(PIP) install -r requirements.txt

clean:
	rm -rf $(VENV)
	find -iname "*.pyc" -delete
	rm -rf __pycache__