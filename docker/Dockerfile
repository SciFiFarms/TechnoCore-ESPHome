ARG BUILD_FROM=esphome/esphome-base-amd64:2.6.0
FROM ${BUILD_FROM}

# First install requirements to leverage caching when requirements don't change
COPY requirements.txt /
RUN pip3 install --no-cache-dir -r /requirements.txt

# Then copy esphome and install
# Had to set voluptuous version manually
# https://github.com/esphome/issues/issues/621
# https://github.com/esphome/issues/issues/591
# TODO: hvac 0.2.17 is pretty out of date.
RUN pip install --no-cache-dir ptvsd==4.1.4 hvac==0.2.17  voluptuous==0.11.5
COPY . .
RUN pip3 install --no-cache-dir -e .

# Settings for dashboard
ENV USERNAME="" PASSWORD=""

# The directory the user should mount their configuration files to
WORKDIR /config
# Set entrypoint to esphome so that the user doesn't have to type 'esphome'
# in every docker command twice
ENTRYPOINT ["esphome"]
# When no arguments given, start the dashboard in the workdir
CMD ["/config", "dashboard"]
