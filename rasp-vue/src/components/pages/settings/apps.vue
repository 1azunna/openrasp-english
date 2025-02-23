<template>
  <div>
    <!-- begin app settings -->
    <div class="card">
      <div class="card-header">
        <h3 class="card-title">
         Application management
        </h3>
      </div>
      <div class="card-body">
        <vue-loading v-if="loading" type="spiningDubbles" color="rgb(90, 193, 221)" :size="{ width: '50px', height: '50px' }" />

        <table v-if="! loading" class="table table-bordered table-hover">
          <thead>
            <th>
              name
            </th>
            <th>
             Language
            </th>
            <th>
             Remarks
            </th>
            <th>
              operating
            </th>
          </thead>
          <tbody>
            <tr v-for="row in data" :key="row.id">
              <td style="min-width: 100px;">
                {{ row.name }}
              </td>
              <td nowrap>
                {{ row.language }}
              </td>
              <td>
                {{ row.description }}
              </td>
              <td nowrap>
                <a href="javascript:" @click.prevent="setCurrentApp(row)">
                Switch
                </a>
                <a href="javascript:" @click="editApp(row, true)">
                  edit
                </a>
                <a href="javascript:" @click="deleteApp(row)">
                  delete
                </a>
              </td>
            </tr>
          </tbody>
        </table>
        <nav v-if="! loading">
          <b-pagination v-model="currentPage" align="center" :total-rows="total" :per-page="10" @change="loadApps" />
        </nav>
      </div>
      <div class="card-footer text-right">
        <div class="d-flex">
          <button class="btn btn-primary" @click="editApp({language: 'java'})">
           Add to
          </button>
          <a class="btn btn-primary pull-right" href="v1/api/app/export" style="margin-left: auto" target="_blank">
            Export
          </a>
        </div>
      </div>
    </div>

    <appEditModal ref="appEditModal" @save="onEdit($event)" />
    <!-- end app settings -->
  </div>
</template>

<script>
import appEditModal from "../../modals/appEditModal";
import { mapGetters, mapActions, mapMutations } from "vuex";
export default {
  name: "AppSettings",
  components: {
    appEditModal
  },
  data: function() {
    return {
      data: [],
      loading: false,
      total: 0,
      currentPage: 1
    };
  },
  computed: {
    ...mapGetters(["current_app", "app_list"])
  },
  watch: {
    current_app() {
      this.loadApps(1);
    }
  },
  mounted: function() {
    this.current_app.id = this.$route.params.app_id;
    if (!this.current_app.id) {
      return
    }
    this.loadApps(1);
  },
  methods: {
    ...mapActions(["loadAppList"]),
    ...mapMutations(["setCurrentApp"]),
    loadApps(page) {
      this.loading = true;
      return this.request
        .post("v1/api/app/get", {
          page: page,
          perpage: 10
        })
        .then(res => {
          this.currentPage = page;
          this.data = res.data;
          this.total = res.total;
          this.loading = false;
        });
    },
    deleteApp(data) {
      if (!confirm("Confirm operation")) {
        return;
      }
      return this.request
        .post("v1/api/app/delete", { id: data.id })
        .then(() => {
          const app =
            data.id === this.current_app.id
              ? this.app_list.find(app => app.id !== data.id)
              : this.current_app;
          this.loadApps(1);
          this.loadAppList(app.id);
        });
    },
    editApp: function(data, is_edit) {
      this.$refs.appEditModal.showModal(
        {
          app_id: data.id,
          name: data.name,
          language: data.language,
          description: data.description
        },
        is_edit
      );
    },
    onEdit({ is_edit, data }) {
      this.request
        .post(is_edit ? "v1/api/app/config" : "v1/api/app", data)
        .then(() => {
          this.loadApps(is_edit ? 1 : this.currentPage);
          this.loadAppList(this.current_app.id);
        });
    }
  }
};
</script>
