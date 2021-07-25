import { AppBar, Toolbar, Typography } from "@material-ui/core";
import React from "react";

export default function Header() {

  return (
    <div className="Header">
      <AppBar position="fixed" style={{backgroundColor: "#6495ed", color: "white"}}>
        <Toolbar>
          <Typography variant="h6"> PM2.5 觀測平台 </Typography>
        </Toolbar>
      </AppBar>
    </div>
  );
}