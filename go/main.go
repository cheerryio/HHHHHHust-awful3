package main

import (
	"fmt"

	"github.com/gin-gonic/gin"
	socketio "github.com/googollee/go-socket.io"
)

// GinMiddleware function
func GinMiddleware(allowOrigin string) gin.HandlerFunc {
	return func(c *gin.Context) {
		c.Writer.Header().Set("Access-Control-Allow-Origin", allowOrigin)
		c.Writer.Header().Set("Access-Control-Allow-Credentials", "false")
		c.Writer.Header().Set("Access-Control-Allow-Methods", "POST, OPTIONS, GET, PUT, DELETE")
		c.Writer.Header().Set("Access-Control-Allow-Headers", "Accept, Authorization, Content-Type, Content-Length, X-CSRF-Token, Token, session, Origin, Host, Connection, Accept-Encoding, Accept-Language, X-Requested-With")

		c.Request.Header.Del("Origin")

		c.Next()
	}
}

func main() {
	router := gin.New()
	server,_ := socketio.NewServer(nil)
	server.OnConnect("/", func(s socketio.Conn) error {
		s.SetContext("")
		fmt.Println("connected:", s.ID())
		s.Join("bcast")
		return nil
	})

	server.OnEvent("/", "sendMessage", func(s socketio.Conn, msg string) {
		fmt.Println("sendBroadcast:", msg)
		server.ForEach("","bcast",func (conn socketio.Conn)  {
			if conn.ID()!= s.ID(){
				conn.Emit("receiveMessage", msg)
			}
		})
	})

	server.OnError("/", func(s socketio.Conn, e error) {
		fmt.Println("meet error:", e)
	})
	server.OnDisconnect("/", func(s socketio.Conn, reason string) {
		fmt.Println("closed", reason)
	})
	go server.Serve()
	defer server.Close()

	router.Use(GinMiddleware("*"))
	router.GET("/socket.io/*any", gin.WrapH(server))
	router.POST("/socket.io/*any", gin.WrapH(server))
	router.Run()
}
