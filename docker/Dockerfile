ARG BUILD_FROM=esphome/esphome-base-amd64:1.7.0
FROM ${BUILD_FROM}

# TODO: hvac 0.2.17 is pretty out of date.
RUN pip install --no-cache-dir ptvsd==4.1.4 hvac==0.2.17 
COPY . .
RUN pip2 install --no-cache-dir -e .

WORKDIR /config
ENTRYPOINT ["esphome"]
CMD ["/config", "dashboard"]
