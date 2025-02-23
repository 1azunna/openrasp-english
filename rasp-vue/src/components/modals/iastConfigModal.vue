<template>
  <div id="setConfigModal" class="modal no-fade" tabindex="-1" role="dialog">
    <div class="modal-dialog modal-lg" role="document">
      <div class="modal-content">
        <div class="modal-header">
          <h5 class="modal-title">
            Task settings
          </h5>
          <button type="button" class="close" data-dismiss="modal" aria-label="Close" />
        </div>
        <div class="modal-body">
            <form class="form-horizontal" id>
              <div class="form-group">
                <div class="row">
                  <div class="col">
                    <label>Please select plugin</label>
                  </div>
                </div>
                <div class="row" style="margin-top: 3px;">
                  <template v-for="(value, key, index) in data.scan_plugin_status">
                      <div class="col">
                        <label class="custom-switch" style="min-width: 200px;">
                          <input v-model="value.enable" type="checkbox" class="custom-switch-input">
                          <span class="custom-switch-indicator"></span>
                          <span class="custom-switch-description">{{value.show_name}}</span>
                        </label>
                      </div>
                  </template>
                </div>
              </div>
              <div class="form-group">
                <div class="row" style="margin-top: 3px;">
                  <div class="col">
                    <label>url whitelist regularity (urls hit by regularization will not be scanned, regular ones will match the request_uri part,
                      For example, http://test.com/path/eg?p=1&b=2 corresponds to the request_uri part /path/eg?p=1&b=2,
                      When you don’t want to scan the path starting with path/eg, you can configure it as ^/path/eg)</label>
                    <input type="text" class="form-control" v-model="data.white_url_reg">
                    <label v-show="byurl_regex_error" style="color:#ff0000">*Whitelist is illegal</label>
                  </div>
                </div>
              </div>
              <div class="form-group">
                <div class="row" style="margin-top: 3px;">
                  <div class="col">
                    <label for="concurrent" align="center">Maximum number of concurrent requests</label>
                    <input type="number" class="form-control" id="concurrent"
                           v-model="data.scan_rate.max_concurrent_request" placeholder="20">
                    <label v-show="data.scan_rate.max_concurrent_request <= 0" style="color:#F00">*The maximum concurrent number should be greater than 0</label>
                  </div>
                </div>
              </div>
              <div class="form-group">
                <div class="row" style="margin-top: 3px;">
                  <div class="col">
                    <label for="minInterval" align="center">Request interval (ms)</label>
                  </div>
                </div>
                <div class="row" style="margin-top: 3px;">
                  <div class="col">
                    <input type="number" class="form-control" id="minInterval"
                           v-model="data.scan_rate.min_request_interval" placeholder="0">
                    <label v-show="data.scan_rate.min_request_interval < 0" style="color:#F00">*The minimum interval should be greater than or equal to 0</label>
                  </div>
                  <div class="col" align="middle">
                    <h5>～</h5>
                  </div>
                  <div class="col">
                    <input type="number" class="form-control" id="maxInterval"
                           v-model="data.scan_rate.max_request_interval" placeholder="1000">
                    <label v-show="data.scan_rate.max_request_interval <= 0" style="color:#F00">*Maximum interval should be greater than 0</label>
                  </div>
                </div>
              </div>
              <div class="form-group">
                <div class="row" style="margin-top: 3px;">
                  <div class="col">
                      <label>Http proxy used by scan request</label>
                    <input type="text" class="form-control"
                           id="scanProxy" v-model="data.scan_proxy">
                  </div>
                </div>
              </div>
            </form>

        </div>
        <div class="modal-footer">
          <button type="button" class="btn btn-primary pull-left"
                  @click="setConfigTask()" :disabled="byurl_regex_error"
                  :data-dismiss=modalDisplay>OK
          </button>
          <button class="btn btn-info" :click="reset()" data-dismiss="modal">
            shut down
          </button>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import { validateRegex } from "@/util"


export default {
  name: 'IastConfigModal',
  data: function() {
    return {
      data: {
          scan_rate: {
              max_concurrent_request: 20,
              max_request_interval: 1000,
              min_request_interval: 0,
          },
          white_url_reg: "",
          scan_proxy: "",
      },
      scan_plugin_status: {},
      byurl_regex_error: false,
      modalDisplay: "modal"
    }
  },
  watch: {
      'data.white_url_reg': function(newVal, oldVal) {
          this.byurl_regex_error = this.validateRegex(newVal)
      }
  },
  methods: {
    showModal(data) {
      this.data = data
      $('#setConfigModal').modal()
    },
    validateRegex,
    getRequest(url, order, data) {
        const body = {
            order: order,
            data: data,
            headers: {'Content-Type': 'application/json'}
        }
        return this.request.post(url, body)
    },
    // set config task
    setConfigTask() {
        var host = this.data.host
        var port = this.data.port
        var app_id = this.data.app_id
        var scanRate = this.data.scan_rate
        this.modalDisplay = "modal";
        this.concurrent = isNaN(Number(scanRate.max_concurrent_request))? undefined : Number(scanRate.max_concurrent_request);
        this.minInterval = isNaN(Number(scanRate.min_request_interval))? undefined : Number(scanRate.min_request_interval);
        this.maxInterval = isNaN(Number(scanRate.max_request_interval))? undefined : Number(scanRate.max_request_interval);
        this.data.white_url_reg == undefined? "" : this.data.white_url_reg;
        this.data.scan_proxy == undefined? "" : this.data.scan_proxy;

        if(!this.byurl_regex_error && this.concurrent > 0 && this.maxInterval > 0 && this.minInterval >= 0){
            if(this.minInterval <= this.maxInterval){
                this.getRequest(this.data.baseUrl, "setConfig", {
                    "host": host,
                    "port": port,
                    "app_id": app_id,
                    "config": {
                        "scan_plugin_status": this.data.scan_plugin_status,
                        "scan_rate": {
                            "max_concurrent_request": this.concurrent,
                            "max_request_interval": this.maxInterval,
                            "min_request_interval": this.minInterval
                        },
                        "white_url_reg": this.data.white_url_reg,
                        "scan_proxy": this.data.scan_proxy
                    }
                }).then(res => {
                    var status = res.status;
                    if(status == 0){
                        alert("设置成功");
                        this.concurrent = undefined;
                        this.minInterval = undefined;
                        this.maxInterval = undefined;
                    }else{
                        alert(res.description);
                    }
                })

            }else{
                this.minInterval = undefined
                alert("最小间隔不应超过最大间隔")
                this.modalDisplay = false;
            }
        }else{
            this.modalDisplay = false;
        }
    },
    setPluginStatus(key) {
        this.data.scan_plugin_status[pl].enable = !this.data.scan_plugin_status[pl].enable
    },
    reset() {
      this.concurrent = undefined;
      this.minInterval = undefined;
      this.maxInterval = undefined;
    }
  }
}
</script>
