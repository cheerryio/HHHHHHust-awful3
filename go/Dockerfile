ARG GO_VERSION=1.15

FROM golang:${GO_VERSION}-alpine AS builder

RUN apk update && apk add alpine-sdk

RUN mkdir -p /go
WORKDIR /go

COPY ./* .

RUN go env -w GO111MODULE=on && \
    go env -w GOPROXY=https://goproxy.io,direct && \
    go mod download

RUN go build -o ./app ./main.go

FROM alpine:latest

RUN apk update && apk add ca-certificates

RUN mkdir -p /go
WORKDIR /go
COPY --from=builder /go/app .

EXPOSE 8080

ENTRYPOINT ["./app"]