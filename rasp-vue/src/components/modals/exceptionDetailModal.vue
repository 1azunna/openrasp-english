<template>
  <div id="showExceptionDetailModal" class="modal no-fade" tabindex="-1" role="dialog">
    <div class="modal-dialog modal-lg" role="document">
      <div class="modal-content">
        <div class="modal-header">
          <h5 class="modal-title">
            Exception details
          </h5>
          <button type="button" class="close" data-dismiss="modal" aria-label="Close" />
        </div>
        <div class="modal-body" style="padding-top: 0">
          <ul id="myTab" class="nav nav-tabs" role="tablist">
            <li class="nav-item">
              <a id="home-tab" class="nav-link active" data-toggle="tab" href="#exception">
                Exception information
              </a>
            </li>
            <li class="nav-item">
              <a class="nav-link" id="profile-tab" data-toggle="tab" href="#profile" role="tab">Asset information</a>
            </li>
          </ul>
          <br>
          <div id="myTabContent" class="tab-content">
            <div id="exception" class="tab-pane fade show active" role="tabpanel" aria-labelledby="home-tab">
              <div class="h6">
                Alarm time
              </div>
              <p>{{ moment(data.event_time).format('YYYY-MM-DD HH:mm:ss') }}</p>
              <div class="h6">
                Alarm message
              </div>
              <p style="word-break: break-all; ">
                [{{ data.error_code }}] {{ data.message }}
              </p>

              <div v-if="data.pid">
                <div class="h6">
                  PID
                </div>
                <p style="word-break: break-all; ">
                  {{ data.pid }}
                </p>
              </div>

              <div v-if="data.path">
                <div class="h6">
                  Log path
                </div>
                <p style="word-break: break-all; ">
                  {{ data.path }}
                </p>
              </div>

              <div class="h6" v-if="data.stack_trace">
                Stack information
              </div>
              <pre v-if="data.stack_trace">{{ data.stack_trace }}</pre>
            </div>

            <div class="tab-pane fade" id="profile" role="tabpanel" aria-labelledby="profile-tab">
              <div class="h6">
               Host name
              </div>
              <p>{{ data.server_hostname }}</p>
              <div class="h6">
                Server IP
              </div>
              <ul>
                <li v-for="nic in data.server_nic" :key="nic.name">{{ nic.name }}: {{ nic.ip }}</li>
              </ul>
            </div>
          </div>
        </div>
        <div class="modal-footer">
          <button class="btn btn-primary" data-dismiss="modal">
           shut down
          </button>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import { attack_type2name } from '../../util'

export default {
  name: 'ExceptionDetailModal',
  data: function() {
    return {
      data: {
      }
    }
  },
  methods: {
    showModal(data) {
      this.data = data

      $('#showExceptionDetailModal').modal()
    }
  }
}
</script>
