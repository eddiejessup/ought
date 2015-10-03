import std.string;

struct State {
    int label = -1;
    int cachedLabel = -1;

    void cache() {
        if (this.label != -1) {
            this.cachedLabel = this.label;
            this.label = -1;
        }
    }

    //int transitionFunc(int selfLabel, int[] labels) {
    //    if (labels.length == 0) {
    //        throw new Exception("Node has no friends, cannot apply this transition function");
    //    } else {
    //        return labels[0];
    //    }
    //}

    //int transitionFunc(int selfLabel, int[] labels) {
    //    return (selfLabel + 1) % 10;
    //}

    int transitionFunc(int selfLabel, int[] labels) {
        const int numAliveForBirth = 3;
        const int minNumAlive = 2;
        const int maxNumAlive = 3;

        const int labelAlive = 1;
        const int labelDead = 0;

        int numAlive = 0;
        foreach (label; labels) {
            numAlive += label == labelAlive ? 1 : 0;
        }

        if (selfLabel == labelAlive) {
            if ((numAlive < minNumAlive) || (numAlive > maxNumAlive)) {
                return labelDead;
            } else {
                return labelAlive;
            }
        } else {
            if (numAlive == numAliveForBirth) {
                return labelAlive;
            } else {
                return labelDead;
            }
        }
    }

    void uncache() {
        this.cachedLabel = -1;
    }

    bool isUpdated() {
        return this.cachedLabel == -1 && this.label != -1;
    }

    bool isCached() {
        return this.cachedLabel != -1 && this.label == -1;
    }

    bool isDirty() {
        return this.cachedLabel != -1 && this.label != -1;
    }

    void iterate(State[] friendStates) {
        int[] cachedLabels;
        foreach (State state; friendStates) {
            cachedLabels ~= state.cachedLabel;
        }
        this.label = this.transitionFunc(this.cachedLabel, cachedLabels);
    }

    string toString() {
        if (this.isUpdated()) {
            return format("State(updated, label=%d)", this.label);
        } else if (this.isCached()) {
            return format("State(cached, cachedLabel=%d)", this.cachedLabel);
        } else if (this.isDirty()) {
            return format("State(dirty, label=%d, cachedLabel=%d)",
                          this.label, this.cachedLabel);
        } else {
            throw new Exception("State is in undefined state");
        }
    }
}

struct Node {
    string name;
    State state;
    Node*[] friends;

    void cache() {
        this.state.cache();
    }

    auto label() {
        return this.state.label;
    }

    void uncache() {
        this.state.uncache();
    }

    void addFriend(Node* node) {
        import std.algorithm.searching : canFind;

        if (this.friends.canFind(node)) {
            throw new Exception("Cannot add friend to node twice");
        } else {
            this.friends ~= node;
        }
    }

    State[] getFriendStates() {
        State[] friendStates;
        foreach (Node* friend; this.friends) {
            friendStates ~= friend.state;
        }
        return friendStates;
    }

    Node[] getFriends() {
        Node[] friends;
        foreach (Node* friendRef; this.friends) {
            friends ~= *friendRef;
        }
        return friends;
    }

    void iterate() {
        this.state.iterate(this.getFriendStates());
    }

    string toString() {
        string[] friendNames;
        foreach (Node* friend; this.friends) {
            friendNames ~= friend.name;
        }
        return format("Node(name=%s, state=%s, friends=%s)",
                      this.name, this.state, friendNames);
    }
}

struct Network {
    Node[] nodes;

    void iterate() {
        
        foreach (ref node; this.nodes) {
        //foreach (ref node; parallel(this.nodes)) {
            node.cache();
        }
        foreach (ref node; this.nodes) {
        //foreach (ref node; parallel(this.nodes)) {
            node.iterate();
        }
        foreach (ref node; this.nodes) {
        //foreach (ref node; parallel(this.nodes)) {
            node.uncache();
        }
    }

    string[] getNames() {
        string[] names;
        foreach (node; this.nodes) {
            names ~= node.name;
        }
        return names;
    }

    string toJSON() {
        import std.json;
        import std.algorithm.searching : countUntil;

        string[] names = this.getNames();
        JSONValue[] nodesJSON, linksJSON;
        foreach (int nodeIndex, node; this.nodes) {
            auto nodeJSON = JSONValue(["name": node.name,
                                       "id": node.name]);
            nodeJSON.object["index"] = nodeIndex;
            nodeJSON.object["state"] = node.label();
            nodesJSON ~= nodeJSON;
            foreach(friend; node.getFriends()) {
                long friendIndex = countUntil(names, friend.name);
                auto linkJSON = JSONValue(["source": nodeIndex,
                                           "target": friendIndex]);
                linksJSON ~= linkJSON;
            }
        }
        auto networkJSON = JSONValue(["nodes": nodesJSON,
                                      "links": linksJSON]);
        return networkJSON.toString();
    }

    string toString() {
        return format("Network(nodes=%s)", this.nodes);
    }
}

ulong getIXY(const ulong iX, const ulong iY, const ulong nY) {
    return iY + iX * nY;
}

Node[] getPeriodicTwoDimLattice(const int[][] initialState) {
    ulong nX = initialState.length;
    ulong nY = initialState[0].length;
    Node[] nodes = new Node[nX * nY];
    foreach (iX; 0..nX) {
        foreach (iY; 0..nY) {
            string name = format("(%d, %d)", iX, iY);
            nodes[getIXY(iX, iY, nY)] = Node(name, State(initialState[iX][iY]));
        }
    }
    foreach (iX; 0..nX) {
        auto iXUp = (iX < nX - 1) ? iX + 1 : 0;
        auto iXDown = (iX > 0) ? iX - 1 : nX - 1;
        foreach (iY; 0..nY) {
            auto iYUp = (iY < nY - 1) ? iY + 1 : 0;
            auto iYDown = (iY > 0) ? iY - 1 : nY - 1;

            Node* node = &nodes[getIXY(iX, iY, nY)];
            node.addFriend(&nodes[getIXY(iXUp, iY, nY)]);
            node.addFriend(&nodes[getIXY(iXDown, iY, nY)]);
            node.addFriend(&nodes[getIXY(iX, iYUp, nY)]);
            node.addFriend(&nodes[getIXY(iX, iYDown, nY)]);
            node.addFriend(&nodes[getIXY(iXUp, iYUp, nY)]);
            node.addFriend(&nodes[getIXY(iXUp, iYDown, nY)]);
            node.addFriend(&nodes[getIXY(iXDown, iYUp, nY)]);
            node.addFriend(&nodes[getIXY(iXDown, iYDown, nY)]);
        }
    }
    return nodes;
}

int[][] getRandomBitArray(int nX, int nY) {
    import std.random;

    int[][] bits = new int[][](nX, nY);
    foreach (int iX; 0..nX) {
        foreach (int iY; 0..nY) {
            bits[iX][iY] = uniform(0, 2);
        }
    }
    return bits;
}
