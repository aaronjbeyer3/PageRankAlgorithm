setwd("C:/Users/Brian/Desktop/PageRank Multithreading")
wd
library(igraph)
g=read.graph(file="toyGraph.txt",directed=TRUE)
n=vcount(g)
V(g)$names = seq(1:n)
sizes=rep(1/n,n)
sizes[1] = 10
sizes[5] = 50
node.size<-setNames(sizes,V(g)$names)

plot(g,vertex.label=V(g)$names,
     edge.arrow.size=0.01,vertex.label.color = "black",vertex.size=as.matrix(node.size) )


X = read.table("myresults.txt")

#number of nodes in x (the 1 stands for number of elements in 1st row)
nn=dim(X)[1]       
              
for(i in 1:nn){
  sizes=X[i,]
  node.size<-setNames(sizes,V(g)$name)
                    
  plot(g,vertex.label=V(g)$names,
      edge.arrow.size=0.01,vertex.label.color = "black",vertex.size=as.matrix(node.size) )
  #Export plot to PDF (for this instance)
}
