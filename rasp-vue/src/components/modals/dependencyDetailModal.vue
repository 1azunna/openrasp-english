<template>
  <div id="showDependencyDetailModal" class="modal no-fade" tabindex="-1" role="dialog">
    <div class="modal-dialog" role="document" style="max-width: 90%">
      <div class="modal-content">
        <div class="modal-header">
          <h5 class="modal-title">
            Dependency details: {{ search_data.tag }}
          </h5>
          <button type="button" class="close" data-dismiss="modal" aria-label="Close" />
        </div>
        <div class="modal-body">
          <table class="table table-bordered">
            <thead>
              <tr>
                <th>Host name</th>
                <th>Network Information</th>
                <th>Final report</th>
                <th>Path list</th>
              </tr>
            </thead>
            <tbody>
              <tr v-for="row in data" :key="row.id">
                <td nowrap>{{ row.hostname }}</td>
                <td nowrap>{{ row.register_ip }}</td>
                <td nowrap>{{ moment(row['@timestamp']).format('YYYY-MM-DD HH:mm:ss') }}</td>
                <td>
                  {{ row.path.join(', ') }}
                </td>
              </tr>
            </tbody>
          </table>

          <ul class="pagination pull-left">
              <li class="active">
                <span style="margin-top: 0.5em; display: block; ">
                  <strong>{{ total }}</strong> The results show that {{ currentPage }} / {{ ceil(total / 10) }} 页
                </span>
              </li>
          </ul>
          <b-pagination v-model="currentPage" align="right" :total-rows="total" :per-page="10" @change="fetchData" />
          
        </div>
        <div class="modal-footer">
          <button class="btn btn-primary" data-dismiss="modal">
            关闭
          </button>
        </div>
      </div>
    </div>
  </div>
</template>

<script>
import { attack_type2name } from '../../util'

export default {
  name: 'DependencyDetailModal',
  data: function() {
    return {
      loading: false,
      total: 0,
      currentPage: 1,
      data: {},
      search_data: {}
    }
  },
  methods: {
    ceil: Math.ceil,
    fetchData(page) {
      this.request.post('v1/api/dependency/search', {
        data: this.search_data,
        page: page,
        perpage: 10
      }).then(res => {
        this.data = res.data
        this.total = res.total
      })
    },

    showModal(data) {
      this.fetchData(1)
      $('#showDependencyDetailModal').modal()
    }
  }
}
</script>
