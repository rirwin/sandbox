FROM pypy:3-onbuild
RUN mkdir /code
ADD ./random_numbers.py /code/random_numbers.py
CMD [ "pypy3", "/code/random_numbers.py" ]
