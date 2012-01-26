
// class my_callback : public saga::callback 
// {
//   public:
//     bool cb (saga::monitorable o,
//              saga::metric      m,
//              saga::context     c)
//     {
//       saga::job::job j = dynamic_cast <saga::job::job> (o);
// 
//       cout << " job " << j.get_id () << " changed state to " << j.get_state () << endl;
// 
//       cout << " job changed state to " << m.get_attribute ("Value") << endl;
// 
//       return true;
//     }
// }
// 
// 
// {
//   saga::task_container tc;
// 
//   saga::job::service js (contact);
// 
//   saga::job::description jd;
//   // ...
//   
//   saga::job::job j1 = js.create_job (jd);
//   // ...
// 
//   saga::job::job j2 = js.create_job (jd);
// 
//   my_callback mycb;
//   j1.add_callback ("job.state",        mycb);
//   j1.add_callback ("job.state_detail", mycb);
// 
//   // state:        "New"
//   // state:        "Running"
//   // state:        "Suspended"
//   // state:        "Done"
//   // state:        "Failed"
//   // state:        "Canceled"
//  
//   // state_detail: "GAT:Running:Globus:StageIn"
//   // state_detail: "DRMAA:Queued:PBS:StageIn"
//   // state_detail: "DRMAA:Running"
//   // state_detail: "Globus:Active"
// 
//   j1.run ();
// 
//   j1.signal (SIGUSR1);
//   j1.signal (SIGTERM);
// 
// 
//   // job inherits task
//   tc.add_task (j1);
//   tc.add_task (j2);
// 
//   tc.run ();
// 
//   while ( tc.size () )
//   {
//     saga::task t = tc.wait (saga::task::Any);
// 
//     cout << " task final: " << t.get_id () << endl;
//   }
// 
// 
// }
// 
// 
// signal_int_to_name (int native)
// {
//   case ( native )
//   {
//     case SIGTERM:
//       return "SIGTERM";
//   }
// 
//   return "OOPS";
// }

int main ()
{
  return 0;
}

