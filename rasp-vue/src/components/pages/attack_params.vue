<template>
    <div class="event-detail">
        <div v-if="data.attack_type == 'sql'">
            <div class="h6">
               Database type
            </div>
            <p>
                {{ data.attack_params.server }}
            </p>
            <div class="h6">
                执行的SQL语句
                <!--
                <a href="javascript:" style="color: #467fcf"
                    v-clipboard:copy="data.attack_params.query"
                    v-clipboard:success="onCopySucc"
                    v-clipboard:error="onCopyError">[copy]</a> 
                -->
            </div>
            <p>
                {{ data.attack_params.query.length > 10000 ? data.attack_params.query + ' ...' : data.attack_params.query }}                
            </p>
        </div>

        <div v-if="data.attack_type == 'sql_exception'">
            <div class="h6">
               Database type
            </div>
            <p>
                {{ data.attack_params.server }}
            </p>            
            <div v-if="data.attack_params.query">
                <div class="h6">
                   SQL statement executed

                    <!--
                    <a href="javascript:" 
                        style="color: #467fcf" 
                        v-clipboard:copy="data.attack_params.query"
                        v-clipboard:success="onCopySucc"
                        v-clipboard:error="onCopyError">复制</a>
                    -->
                </div>
                <p>
                    {{ data.attack_params.query.length > 10000 ? data.attack_params.query + ' ...' : data.attack_params.query }}
                </p>
            </div>
            <!--
            <div class="h6">
               Error message
            </div>
            <p>
                [{{ data.attack_params.error_code }}] {{ data.attack_params.error_msg }}
            </p>
            -->
        </div>        

        <div v-if="data.attack_type == 'directory'">
            <div class="h6">
                Read directory
            </div>
            <p>
                {{ data.attack_params.path }}
            </p>
            <div class="h6">
              Read directory-real path
            </div>
            <p>
                {{ data.attack_params.realpath }}
            </p>
        </div>        

        <div v-if="data.attack_type == 'readFile'">
            <div class="h6">
              File read
            </div>
            <p>
                {{ data.attack_params.path }}
            </p>
            <div class="h6">
                File read-real path
            </div>
            <p>
                {{ data.attack_params.realpath }}
            </p>
        </div>      

        <div v-if="data.attack_type == 'writeFile'">
            <div class="h6">
               File written
            </div>
            <p>
                {{ data.attack_params.path }}
            </p>
            <div class="h6">
              File written-real path
            </div>
            <p>
                {{ data.attack_params.realpath }}
            </p>
        </div>     

        <div v-if="data.attack_type == 'deleteFile'">
            <div class="h6">
              Deleted files
            </div>
            <p>
                {{ data.attack_params.path }}
            </p>
            <div class="h6">
             Deleted file-real path
            </div>
            <p>
                {{ data.attack_params.realpath }}
            </p>
        </div>      

        <div v-if="data.attack_type == 'include'">
            <div class="h6">
              Files to include
            </div>
            <p>
                {{ data.attack_params.url }}
            </p>
            <div class="h6" v-if="data.attack_params.realpath">
             File to include-real path
            </div>
            <p>
                {{ data.attack_params.realpath }}
            </p>            
        </div>     

        <div v-if="data.attack_type == 'webdav'">
            <div class="h6">
               Source File
            </div>
            <p>
                {{ data.attack_params.source }}
            </p>
            <div class="h6">
               Target file
            </div>
            <p>
                {{ data.attack_params.dest }}
            </p>            
        </div>                    

        <div v-if="data.attack_type == 'fileUpload'">
            <div class="h6" v-if="data.attack_params.name">
               Multipart parameter name
            </div>
            <p v-if="data.attack_params.name">
                {{ data.attack_params.name }}
            </p>

            <div class="h6">
                Uploaded file name
            </div>
            <p>
                {{ data.attack_params.filename }}
            </p>

            <template v-if="data.attack_params.dest_path">
                <div class="h6">
                   File storage location
                </div>
                <p>{{ data.attack_params.dest_path }}</p>

                <div class="h6">
                 File storage location (real path)
                </div>
                <p>{{ data.attack_params.dest_realpath }}</p>
            </template>
            
            <div class="h6">
                Uploaded file content-first 4KB
            </div>
            <pre>{{ data.attack_params.content }}</pre>

            
        </div>

        <div v-if="data.attack_type == 'rename'">
            <div class="h6">
               Source File
            </div>
            <p>
                {{ data.attack_params.source }}
            </p>
            <div class="h6">
                Target file
            </div>
            <p>
                {{ data.attack_params.dest }}
            </p>
        </div>        

        <div v-if="data.attack_type == 'command'">
            <div class="h6">
                Command to execute
            </div>
            <p>
                {{ data.attack_params.command }}
            </p>
        </div>  

        <div v-if="data.attack_type == 'xxe'">
            <div class="h6">
                External entity to load
            </div>
            <p>
                {{ data.attack_params.entity }}
            </p>
        </div>         

        <div v-if="data.attack_type == 'ognl'">
            <div class="h6">
                OGNL expression to be executed
            </div>
            <p>
                {{ data.attack_params.expression }}
            </p>
        </div>   

        <div v-if="data.attack_type == 'deserialization'">
            <div class="h6">
                Java class name to be generated
            </div>
            <p>
                {{ data.attack_params.clazz }}
            </p>
        </div>         

        <div v-if="data.attack_type == 'ssrf' || data.attack_type == 'ssrfRedirect'">
            <div class="h6">
                URL to visit
            </div>
            <p>
                {{ data.attack_params.url }}
            </p>
            <div class="h6">
                Target IP
            </div>
            <p>
                {{ data.attack_params.ip.join(", ") }}
            </p>                     
        </div>

        <div v-if="data.attack_type == 'xss_echo' || data.attack_type == 'xss_userinput'">
            <div class="h6" v-if="data.attack_params.name">
                XSS parameter name
            </div>
            <p v-if="data.attack_params.name">
                {{ data.attack_params.name }}
            </p>

            <div class="h6" v-if="data.attack_params.value">
                XSS exploit code
            </div>
            <p v-if="data.attack_params.value">
                {{ data.attack_params.value.length > 10000 ? data.attack_params.value + ' ...' : data.attack_params.value }}   
            </p>            
        </div>

        <div v-if="data.attack_type == 'response' && data.plugin_algorithm == 'response_dataLeak'">
            <div class="h6">
                Examples of leaked sensitive information
            </div>
            <div>
                <table class="table table-bordered">
                    <thead>
                        <tr>
                            <th>Type</th>
                            <th>Uncoded information</th>
                            <th>Original content (fragment)</th>
                        </tr>
                    </thead>
                    <tbody>
                        <tr v-for="(row, index) in data.attack_params.parts" :key="index">
                            <td nowrap>{{ row.type }}</td>
                            <td nowrap>{{ row.match }}</td>
                            <td>{{ row.parts }}</td>
                        </tr>
                    </tbody>
                </table>
            </div>            
        </div>  

        <!-- 以下为 php 原生 -->

        <div v-if="data.attack_type == 'webshell_eval'">
            <div class="h6">
                The content of eval
            </div>
            <p>
                {{ data.attack_params.eval }}
            </p>
        </div>                  

        <div v-if="data.attack_type == 'webshell_command'">
            <div class="h6">
                Command to be executed by WebShell
            </div>
            <p>
                {{ data.attack_params.command }}
            </p>
        </div>  

        <div v-if="data.attack_type == 'webshell_file_put_contents'">
            <div class="h6">
                File to be written by WebShell
            </div>
            <p>
                {{ data.attack_params.name }}
            </p>
            <div class="h6">
                File to be written by WebShell-real path
            </div>
            <p>
                {{ data.attack_params.realpath }}
            </p>
        </div>
        
        <div v-if="data.attack_type == 'webshell_callable'">
            <div class="h6">
                The function to be executed by the backdoor
            </div>
            <p>
                {{ data.attack_params.function }}()
            </p>
        </div>

    </div>

</template>

<script>

export default {
    name: 'attack_params',
    data: function () {
        return {
            data: {
                message: 'wtf',
                attack_params: {}
            }
        }
    },
    methods: {
        setData: function (data) {
            this.data = data
        },
        onCopySucc: function(e) {
            console.log('succ')
        },
        onCopyError: function(e) {
            console.log('fail', e)
        }
    }
}
</script>

