package id.dwiilham.portanitech10

import android.os.Bundle
import android.util.Log
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import androidx.navigation.findNavController
import androidx.navigation.ui.AppBarConfiguration
import androidx.navigation.ui.setupWithNavController
import com.android.volley.Request
import com.android.volley.Response
import com.android.volley.toolbox.StringRequest
import com.android.volley.toolbox.Volley
import com.google.android.material.bottomnavigation.BottomNavigationView
import com.google.android.material.snackbar.Snackbar
import com.google.gson.Gson
import id.dwiilham.portanitech10.model.callback.AccessTokenCallback
import id.dwiilham.portanitech10.rest.Preferences
import id.dwiilham.portanitech10.rest.RestAdapter
import id.dwiilham.portanitech10.rest.RestInterface
import org.json.JSONObject
import retrofit2.Call
import retrofit2.Callback

class MainActivity : AppCompatActivity() {

    var accessToken: String = ""
    var JWTToken: String = ""
    var deviceId: Int = 26583

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        val navView: BottomNavigationView = findViewById(R.id.nav_view)

        val navController = findNavController(R.id.nav_host_fragment)
        // Passing each menu ID as a set of Ids because each
        // menu should be considered as top level destinations.
        val appBarConfiguration = AppBarConfiguration(setOf(
            R.id.navigation_home, R.id.navigation_dashboard, R.id.navigation_notifications, R.id.navigation_shop, R.id.navigation_help))
        navView.setupWithNavController(navController)

        val linkTrang = "https://flexiot.xl.co.id/api/applicationmgt/authenticate"
        val link1 = "https://flexiot.xl.co.id/api/usermgt/v1/authenticate"

        val queue = Volley.newRequestQueue(this)

        var gson = Gson()

        var respons: AuthPostModel

        val stringRequest = object: StringRequest(
            Request.Method.GET, linkTrang,
            Response.Listener<String> { response ->
                val resp = gson.fromJson(response, AuthModel::class.java)
                Log.d("A", "Response 1 is: " + resp.access_token)
                accessToken = resp.access_token
                val stringRequest1 = object: StringRequest(Request.Method.POST, link1, Response.Listener<String> { response ->
                    respons = gson.fromJson(response, AuthPostModel::class.java)
                    Log.d("B", "Response 2 is " + respons.xIoTJWT)
                    JWTToken = respons.xIoTJWT

                },
                    Response.ErrorListener { error ->
                        Log.e("VOLLEY", error.toString())
                    }){
                    override fun getHeaders(): MutableMap<String, String> {
                        val headers = HashMap<String, String>()
                        headers["Authorization"] = "Bearer " + resp.access_token
                        Log.d("auth", "Bearer " + resp.access_token)
                        return headers
                    }
                    override fun getBodyContentType(): String {
                        return "application/json"
                    }
                    override fun getBody(): ByteArray? {
                        val bodys = HashMap<String, String>()
                        bodys["username"] = "xxxxx@gmail.com"
                        bodys["password"] = "xxxxxx"
                        return JSONObject(bodys as Map<*, *>).toString().toByteArray()

                    }
                }
                queue.add(stringRequest1)
            },
            Response.ErrorListener {  })
        {
            override fun getHeaders(): MutableMap<String, String> {
                val headers = HashMap<String, String>()
                headers["X-secret"] = "aDVvR18yenhtSTNtWHFHRHpGWnQxZ1dnMDZFYTpyYkhmekUzYXhWY2hMSkxDTjRmektrSlIxazRh"
                return headers
            }
        }
        queue.add(stringRequest)

    }


    fun getMyData (): Bundle {
        val bundle = Bundle()
        bundle.putString("access_token", accessToken)
        bundle.putString("X-IoT-JWT", JWTToken)
        bundle.putInt("deviceId", deviceId)

        return bundle
    }

}
